// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <ctime>

#include <boost/make_shared.hpp>

#include <gauge/gauge.hpp>
#include <gauge/console_printer.hpp>
#include <gauge/python_printer.hpp>
#include <gauge/csv_printer.hpp>
#include <gauge/json_printer.hpp>

#include <kodo/has_deep_symbol_storage.hpp>
#include <kodo/has_shallow_symbol_storage.hpp>
#include <kodo/has_systematic_encoder.hpp>
#include <kodo/set_systematic_off.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/rlnc/perpetual_codes.hpp>

#include <fifi/is_prime2325.hpp>
#include <fifi/prime2325_binary_search.hpp>
#include <fifi/prime2325_apply_prefix.hpp>

#include <tables/table.hpp>

/// A test block represents an encoder and decoder pair
template<class Encoder, class Decoder>
struct throughput_benchmark : public gauge::time_benchmark
{
    typedef typename Encoder::factory encoder_factory;
    typedef typename Encoder::factory::pointer encoder_ptr;

    typedef typename Decoder::factory decoder_factory;
    typedef typename Decoder::factory::pointer decoder_ptr;

    void init()
    {
        m_factor = 1;
        gauge::time_benchmark::init();
    }

    void start()
    {
        m_encoded_symbols = 0;
        m_recovered_symbols = 0;
        m_processed_symbols = 0;
        gauge::time_benchmark::start();
    }

    void stop()
    {
        gauge::time_benchmark::stop();
    }

    double calculate_throughput(bool goodput = false)
    {
        // Get the time spent per iteration
        double time = gauge::time_benchmark::measurement();

        gauge::config_set cs = get_current_configuration();
        std::string type = cs.get_value<std::string>("type");
        uint32_t symbol_size = cs.get_value<uint32_t>("symbol_size");

        // The number of bytes {en|de}coded
        uint64_t total_bytes = 0;

        if (type == "decoder")
        {
            if (goodput)
                total_bytes = m_recovered_symbols * symbol_size;
            else
                total_bytes = m_processed_symbols * symbol_size;
        }
        else if (type == "encoder")
        {
            total_bytes = m_encoded_symbols * symbol_size;
        }
        else
        {
            assert(0);
        }

        // The bytes per iteration
        uint64_t bytes =
            total_bytes / gauge::time_benchmark::iteration_count();

        return bytes / time; // MB/s for each iteration
    }

    void store_run(tables::table& results)
    {
        if (!results.has_column("throughput"))
            results.add_column("throughput");

        if (!results.has_column("goodput"))
            results.add_column("goodput");

        results.set_value("throughput", calculate_throughput(false));
        results.set_value("goodput", calculate_throughput(true));
    }

    bool accept_measurement()
    {
        gauge::config_set cs = get_current_configuration();

        std::string type = cs.get_value<std::string>("type");

        if (type == "decoder")
        {
            // If we are benchmarking a decoder we only accept
            // the measurement if the decoding was successful
            if (!m_decoder->is_complete())
            {
                // We did not generate enough payloads to decode successfully,
                // so we will generate more payloads for next run
                ++m_factor;

                return false;
            }

            // At this point, the output data should be equal to the input data
            assert(m_data_out == m_data_in);
        }

        return gauge::time_benchmark::accept_measurement();
    }

    std::string unit_text() const
    {
        return "MB/s";
    }

    void get_options(gauge::po::variables_map& options)
    {
        auto symbols = options["symbols"].as<std::vector<uint32_t> >();
        auto symbol_size = options["symbol_size"].as<std::vector<uint32_t> >();
        auto types = options["type"].as<std::vector<std::string> >();

        assert(symbols.size() > 0);
        assert(symbol_size.size() > 0);
        assert(types.size() > 0);

        for (uint32_t i = 0; i < symbols.size(); ++i)
        {
            for (uint32_t j = 0; j < symbol_size.size(); ++j)
            {
                for (uint32_t u = 0; u < types.size(); ++u)
                {
                    gauge::config_set cs;
                    cs.set_value<uint32_t>("symbols", symbols[i]);
                    cs.set_value<uint32_t>("symbol_size", symbol_size[j]);
                    cs.set_value<std::string>("type", types[u]);

                    add_configuration(cs);
                }
            }
        }
    }

    void setup()
    {
        gauge::config_set cs = get_current_configuration();

        uint32_t symbols = cs.get_value<uint32_t>("symbols");
        uint32_t symbol_size = cs.get_value<uint32_t>("symbol_size");

        /// @todo Research the cause of this
        // Make the factories fit perfectly otherwise there seems to
        // be problems with memory access i.e. when using a factory
        // with max symbols 1024 with a symbols 16
        m_decoder_factory = std::make_shared<decoder_factory>(
            symbols, symbol_size);

        m_encoder_factory = std::make_shared<encoder_factory>(
            symbols, symbol_size);

        m_decoder_factory->set_symbols(symbols);
        m_decoder_factory->set_symbol_size(symbol_size);

        m_encoder_factory->set_symbols(symbols);
        m_encoder_factory->set_symbol_size(symbol_size);

        m_encoder = m_encoder_factory->build();
        m_decoder = m_decoder_factory->build();

        // Prepare the data buffers
        m_data_in.resize(m_encoder->block_size());
        m_data_out.resize(m_encoder->block_size());

        std::generate_n(m_data_in.begin(), m_data_in.size(), rand);

        m_encoder->set_symbols(sak::storage(m_data_in));

        // If the decoder uses shallow storage we have to initialize
        // its decoding buffers
        if (kodo::has_shallow_symbol_storage<Decoder>::value)
        {
            m_decoder->set_symbols(sak::storage(m_data_out));
        }

        // Create the payload buffer
        m_temp_payload.resize(m_decoder->payload_size());

        // Prepare storage to the encoded payloads
        uint32_t payload_count = symbols * m_factor;
        assert(payload_count > 0);

        m_payloads.resize(payload_count);
        for (auto& payload : m_payloads)
        {
            payload.resize(m_encoder->payload_size());
        }
    }

    void encode_payloads()
    {
        // If we are working in the prime2325 finite field we have to
        // "map" the data first. This cost is included in the encoder
        // throughput (we do it with the clock running), just as it
        // would be in a real application.
        if (fifi::is_prime2325<typename Encoder::field_type>::value)
        {
            uint32_t block_length =
                fifi::size_to_length<fifi::prime2325>(m_encoder->block_size());

            fifi::prime2325_binary_search search(block_length);
            m_prefix = search.find_prefix(sak::storage(m_data_in));

            // Apply the negated prefix
            fifi::apply_prefix(sak::storage(m_data_in), ~m_prefix);
        }

        m_encoder->set_symbols(sak::storage(m_data_in));

        // We switch any systematic operations off so we code
        // symbols from the beginning
        if (kodo::has_systematic_encoder<Encoder>::value)
            kodo::set_systematic_off(m_encoder);

        for (auto& payload : m_payloads)
        {
            m_encoder->encode(payload.data());
            ++m_encoded_symbols;
        }

        /// @todo Revert to the original input data by re-applying the
        /// prefix to the input data. This is needed since the
        /// benchmark loops and re-encodes the same data. If we did
        /// not re-apply the prefix the input data would be corrupted
        /// in the second iteration, causing decoding to produce
        /// incorrect output data. In a real-world application this
        /// would typically not be needed. For this reason the
        /// prime2325 results encoding results will show a lower
        /// throughput than what we can expect to see in an actual
        /// application. Currently we don't have a good solution for
        /// this. Possible solutions would be to copy the input data,
        /// however in that way we will also see a performance hit.
        if (fifi::is_prime2325<typename Encoder::field_type>::value)
        {
            // Apply the negated prefix
            fifi::apply_prefix(sak::storage(m_data_in), ~m_prefix);
        }
    }

    void decode_payloads()
    {
        for (const auto& payload : m_payloads)
        {
            /// @todo This copy would typically not be performed by an
            /// actual application, however since the benchmark
            /// performs several iterations while decoding the same data we
            /// have to copy the input data to avoid corrupting it.
            /// The decoder works on the data "in-place", therefore we
            /// would corrupt the payloads if we did not copy them.
            ///
            /// Changing this would most likely improve the throughput
            /// of the decoders. We are open to suggestions :)
            std::copy_n(payload.data(), payload.size(), m_temp_payload.data());

            m_decoder->decode(m_temp_payload.data());

            ++m_processed_symbols;

            if (m_decoder->is_complete())
            {
                // If the decoder uses deep storage, we have to copy
                // its decoding buffers for verification.
                // This would also be necessary in real applications, so it
                // should be part of the timed benchmark.
                if (kodo::has_deep_symbol_storage<Decoder>::value)
                {
                    m_decoder->copy_symbols(sak::storage(m_data_out));
                }

                if(fifi::is_prime2325<typename Decoder::field_type>::value)
                {
                    // Now we have to apply the negated prefix to the
                    // decoded data
                    fifi::apply_prefix(sak::storage(m_data_out), ~m_prefix);
                }

                m_recovered_symbols += m_decoder->symbols();

                return;
            }
        }
    }

    /// Run the encoder
    void run_encode()
    {
        gauge::config_set cs = get_current_configuration();

        uint32_t symbols = cs.get_value<uint32_t>("symbols");
        uint32_t symbol_size = cs.get_value<uint32_t>("symbol_size");

        m_encoder_factory->set_symbols(symbols);
        m_encoder_factory->set_symbol_size(symbol_size);

        // The clock is running
        RUN
        {
            // We have to make sure the encoder is in a "clean" state
            m_encoder->initialize(*m_encoder_factory);

            encode_payloads();
        }
    }

    /// Run the decoder
    void run_decode()
    {
        // Encode some data
        encode_payloads();

        gauge::config_set cs = get_current_configuration();

        uint32_t symbols = cs.get_value<uint32_t>("symbols");
        uint32_t symbol_size = cs.get_value<uint32_t>("symbol_size");

        m_decoder_factory->set_symbols(symbols);
        m_decoder_factory->set_symbol_size(symbol_size);

        // Zero the data buffer for the decoder
        std::fill_n(m_data_out.begin(), m_data_out.size(), 0);

        // The clock is running
        RUN
        {
            // We have to make sure the decoder is in a "clean" state
            // i.e. no symbols already decoded.
            m_decoder->initialize(*m_decoder_factory);

            if (kodo::has_shallow_symbol_storage<Decoder>::value)
            {
                m_decoder->set_symbols(sak::storage(m_data_out));
            }

            // Decode the payloads
            decode_payloads();
        }
    }

    void run_benchmark()
    {
        gauge::config_set cs = get_current_configuration();

        std::string type = cs.get_value<std::string>("type");

        if (type == "encoder")
        {
            run_encode();
        }
        else if (type == "decoder")
        {
            run_decode();
        }
        else
        {
            assert(0);
        }
    }

protected:

    /// The decoder factory
    std::shared_ptr<decoder_factory> m_decoder_factory;

    /// The encoder factory
    std::shared_ptr<encoder_factory> m_encoder_factory;

    /// The encoder to use
    encoder_ptr m_encoder;

    /// The number of encoded symbols
    uint32_t m_encoded_symbols;

    /// The decoder to use
    decoder_ptr m_decoder;

    /// The number of symbols recovered by the decoder
    uint32_t m_recovered_symbols;

    /// The number of symbols processed by the decoder
    uint32_t m_processed_symbols;

    /// The input data
    std::vector<uint8_t> m_data_in;

    /// The output data
    std::vector<uint8_t> m_data_out;

    /// Buffer storing the encoded payloads before adding them to the
    /// decoder. This is necessary since the decoder will "work on"
    /// the encoded payloads directly. Therefore if we want to be able
    /// to run multiple iterations with the same encoded paylaods we
    /// have to copy them before injecting them into the decoder. This
    /// of course has a negative impact on the decoding throughput.
    std::vector<uint8_t> m_temp_payload;

    /// Storage for encoded symbols
    std::vector< std::vector<uint8_t> > m_payloads;

    /// Multiplication factor for payload_count
    uint32_t m_factor;

    /// Prefix used when testing with the prime2325 finite field
    uint32_t m_prefix;
};


/// A test block represents an encoder and decoder pair
template<class Encoder, class Decoder>
struct sparse_throughput_benchmark :
    public throughput_benchmark<Encoder,Decoder>
{
public:

    /// The type of the base benchmark
    typedef throughput_benchmark<Encoder,Decoder> Super;

    /// We need access to the encoder built to adjust the average number of
    /// nonzero symbols
    using Super::m_encoder;

public:

    void get_options(gauge::po::variables_map& options)
    {
        auto symbols = options["symbols"].as<std::vector<uint32_t> >();
        auto symbol_size = options["symbol_size"].as<std::vector<uint32_t> >();
        auto types = options["type"].as<std::vector<std::string> >();
        auto density = options["density"].as<std::vector<double> >();

        assert(symbols.size() > 0);
        assert(symbol_size.size() > 0);
        assert(types.size() > 0);
        assert(density.size() > 0);

        for (const auto& s : symbols)
        {
            for (const auto& p : symbol_size)
            {
                for (const auto& t : types)
                {
                    for (const auto& d: density)
                    {
                        gauge::config_set cs;
                        cs.set_value<uint32_t>("symbols", s);
                        cs.set_value<uint32_t>("symbol_size", p);
                        cs.set_value<std::string>("type", t);
                        cs.set_value<double>("density", d);

                        Super::add_configuration(cs);
                    }
                }
            }
        }
    }

    void setup()
    {
        Super::setup();

        gauge::config_set cs = Super::get_current_configuration();
        double symbols = cs.get_value<double>("density");
        m_encoder->set_density(symbols);
    }
};

/// A test block represents an encoder and decoder pair
template<class Encoder, class Decoder>
struct perpetual_throughput_benchmark :
    public throughput_benchmark<Encoder,Decoder>
{
public:

    /// The type of the base benchmark
    typedef throughput_benchmark<Encoder,Decoder> Super;

    /// We need access to the encoder built to adjust the average number of
    /// nonzero symbols
    using Super::m_encoder;

public:

    void get_options(gauge::po::variables_map& options)
    {
        auto symbols = options["symbols"].as<std::vector<uint32_t> >();
        auto symbol_size = options["symbol_size"].as<std::vector<uint32_t> >();
        auto types = options["type"].as<std::vector<std::string> >();
        auto width_ratio = options["width_ratio"].as<std::vector<double> >();

        assert(symbols.size() > 0);
        assert(symbol_size.size() > 0);
        assert(types.size() > 0);
        assert(width_ratio.size() > 0);

        for (const auto& s : symbols)
        {
            for (const auto& p : symbol_size)
            {
                for (const auto& t : types)
                {
                    for (const auto& w: width_ratio)
                    {
                        gauge::config_set cs;
                        cs.set_value<uint32_t>("symbols", s);
                        cs.set_value<uint32_t>("symbol_size", p);
                        cs.set_value<std::string>("type", t);
                        cs.set_value<double>("width_ratio", w);

                        Super::add_configuration(cs);
                    }
                }
            }
        }
    }

    void setup()
    {
        Super::setup();

        gauge::config_set cs = Super::get_current_configuration();
        double width_ratio = cs.get_value<double>("width_ratio");
        m_encoder->set_width_ratio(width_ratio);
    }
};

/// Using this macro we may specify options. For specifying options
/// we use the boost program options library. So you may additional
/// details on how to do it in the manual for that library.
BENCHMARK_OPTION(throughput_options)
{
    gauge::po::options_description options;

    std::vector<uint32_t> symbols;
    symbols.push_back(16);
    symbols.push_back(32);
    symbols.push_back(64);
    symbols.push_back(128);
    symbols.push_back(256);
    symbols.push_back(512);

    auto default_symbols =
        gauge::po::value<std::vector<uint32_t> >()->default_value(
            symbols, "")->multitoken();

    std::vector<uint32_t> symbol_size;
    symbol_size.push_back(1600);

    auto default_symbol_size =
        gauge::po::value<std::vector<uint32_t> >()->default_value(
            symbol_size, "")->multitoken();

    std::vector<std::string> types;
    types.push_back("encoder");
    types.push_back("decoder");

    auto default_types =
        gauge::po::value<std::vector<std::string> >()->default_value(
            types, "")->multitoken();

    options.add_options()
        ("symbols", default_symbols, "Set the number of symbols");

    options.add_options()
        ("symbol_size", default_symbol_size, "Set the symbol size in bytes");

    options.add_options()
        ("type", default_types, "Set type [encoder|decoder]");

    gauge::runner::instance().register_options(options);
}

BENCHMARK_OPTION(sparse_density_options)
{
    gauge::po::options_description options;

    std::vector<double> density;
    density.push_back(0.2);
    density.push_back(0.3);
    density.push_back(0.4);
    density.push_back(0.5);

    auto default_density =
        gauge::po::value<std::vector<double> >()->default_value(
            density, "")->multitoken();

    options.add_options()
        ("density", default_density, "Set the density of the sparse codes");

    gauge::runner::instance().register_options(options);
}

BENCHMARK_OPTION(perpetual_options)
{
    gauge::po::options_description options;

    std::vector<double> width_ratio;
    width_ratio.push_back(0.05);
    width_ratio.push_back(0.1);

    auto default_width_ratio =
        gauge::po::value<std::vector<double> >()->default_value(
            width_ratio, "")->multitoken();

    options.add_options()
        ("width_ratio", default_width_ratio,
        "Set the width for perpetual codes (in percentage)");

    gauge::runner::instance().register_options(options);
}

//------------------------------------------------------------------
// Shallow FullRLNC
//------------------------------------------------------------------

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::binary>,
    kodo::shallow_full_rlnc_decoder<fifi::binary> > setup_rlnc_throughput;

BENCHMARK_F(setup_rlnc_throughput, FullRLNC, Binary, 5)
{
    run_benchmark();
}

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::binary8>,
    kodo::shallow_full_rlnc_decoder<fifi::binary8> > setup_rlnc_throughput8;

BENCHMARK_F(setup_rlnc_throughput8, FullRLNC, Binary8, 5)
{
    run_benchmark();
}

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::binary16>,
    kodo::shallow_full_rlnc_decoder<fifi::binary16> > setup_rlnc_throughput16;

BENCHMARK_F(setup_rlnc_throughput16, FullRLNC, Binary16, 5)
{
    run_benchmark();
}

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::prime2325>,
    kodo::shallow_full_rlnc_decoder<fifi::prime2325> > setup_rlnc_throughput2325;

BENCHMARK_F(setup_rlnc_throughput2325, FullRLNC, Prime2325, 5)
{
    run_benchmark();
}

//------------------------------------------------------------------
// Shallow BackwardFullRLNC
//------------------------------------------------------------------

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::binary>,
    kodo::shallow_backward_full_rlnc_decoder<fifi::binary> >
    setup_backward_rlnc_throughput;

BENCHMARK_F(setup_backward_rlnc_throughput, BackwardFullRLNC, Binary, 5)
{
    run_benchmark();
}

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::binary8>,
    kodo::shallow_backward_full_rlnc_decoder<fifi::binary8> >
    setup_backward_rlnc_throughput8;

BENCHMARK_F(setup_backward_rlnc_throughput8, BackwardFullRLNC, Binary8, 5)
{
    run_benchmark();
}

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::binary16>,
    kodo::shallow_backward_full_rlnc_decoder<fifi::binary16> >
    setup_backward_rlnc_throughput16;

BENCHMARK_F(setup_backward_rlnc_throughput16, BackwardFullRLNC, Binary16, 5)
{
    run_benchmark();
}

typedef throughput_benchmark<
    kodo::shallow_full_rlnc_encoder<fifi::prime2325>,
    kodo::shallow_backward_full_rlnc_decoder<fifi::prime2325> >
    setup_backward_rlnc_throughput2325;

BENCHMARK_F(setup_backward_rlnc_throughput2325, BackwardFullRLNC, Prime2325, 5)
{
    run_benchmark();
}

//------------------------------------------------------------------
// Shallow FullDelayedRLNC
//------------------------------------------------------------------

typedef throughput_benchmark<
   kodo::shallow_full_rlnc_encoder<fifi::binary>,
   kodo::shallow_delayed_full_rlnc_decoder<fifi::binary> >
   setup_delayed_rlnc_throughput;

BENCHMARK_F(setup_delayed_rlnc_throughput, FullDelayedRLNC, Binary, 5)
{
   run_benchmark();
}

typedef throughput_benchmark<
   kodo::shallow_full_rlnc_encoder<fifi::binary8>,
   kodo::shallow_delayed_full_rlnc_decoder<fifi::binary8> >
   setup_delayed_rlnc_throughput8;

BENCHMARK_F(setup_delayed_rlnc_throughput8, FullDelayedRLNC, Binary8, 5)
{
   run_benchmark();
}

typedef throughput_benchmark<
   kodo::shallow_full_rlnc_encoder<fifi::binary16>,
   kodo::shallow_delayed_full_rlnc_decoder<fifi::binary16> >
   setup_delayed_rlnc_throughput16;

BENCHMARK_F(setup_delayed_rlnc_throughput16, FullDelayedRLNC, Binary16, 5)
{
   run_benchmark();
}

typedef throughput_benchmark<
   kodo::shallow_full_rlnc_encoder<fifi::prime2325>,
   kodo::shallow_delayed_full_rlnc_decoder<fifi::prime2325> >
   setup_delayed_rlnc_throughput2325;

BENCHMARK_F(setup_delayed_rlnc_throughput2325, FullDelayedRLNC, Prime2325, 5)
{
   run_benchmark();
}

//------------------------------------------------------------------
// Shallow SparseFullRLNC
//------------------------------------------------------------------

typedef sparse_throughput_benchmark<
    kodo::shallow_sparse_full_rlnc_encoder<fifi::binary>,
    kodo::shallow_full_rlnc_decoder<fifi::binary>>
    setup_sparse_rlnc_throughput;

BENCHMARK_F(setup_sparse_rlnc_throughput, SparseFullRLNC, Binary, 5)
{
    run_benchmark();
}

typedef sparse_throughput_benchmark<
    kodo::shallow_sparse_full_rlnc_encoder<fifi::binary8>,
    kodo::shallow_full_rlnc_decoder<fifi::binary8>>
    setup_sparse_rlnc_throughput8;

BENCHMARK_F(setup_sparse_rlnc_throughput8, SparseFullRLNC, Binary8, 5)
{
    run_benchmark();
}

typedef sparse_throughput_benchmark<
    kodo::shallow_sparse_full_rlnc_encoder<fifi::binary16>,
    kodo::shallow_full_rlnc_decoder<fifi::binary16>>
    setup_sparse_rlnc_throughput16;

BENCHMARK_F(setup_sparse_rlnc_throughput16, SparseFullRLNC, Binary16, 5)
{
    run_benchmark();
}

//------------------------------------------------------------------
// Shallow Perpetual RLNC
//------------------------------------------------------------------

typedef perpetual_throughput_benchmark<
    kodo::shallow_perpetual_encoder<fifi::binary>,
    kodo::shallow_perpetual_decoder<fifi::binary>>
    setup_perpetual_throughput;

BENCHMARK_F(setup_perpetual_throughput, Perpetual, Binary, 5)
{
    run_benchmark();
}

typedef perpetual_throughput_benchmark<
    kodo::shallow_perpetual_encoder<fifi::binary8>,
    kodo::shallow_perpetual_decoder<fifi::binary8>>
    setup_perpetual_throughput8;

BENCHMARK_F(setup_perpetual_throughput8, Perpetual, Binary8, 5)
{
    run_benchmark();
}

typedef perpetual_throughput_benchmark<
    kodo::shallow_perpetual_encoder<fifi::binary16>,
    kodo::shallow_perpetual_decoder<fifi::binary16>>
    setup_perpetual_throughput16;

BENCHMARK_F(setup_perpetual_throughput16, Perpetual, Binary16, 5)
{
    run_benchmark();
}

typedef perpetual_throughput_benchmark<
    kodo::shallow_perpetual_encoder<fifi::prime2325>,
    kodo::shallow_perpetual_decoder<fifi::prime2325>>
    setup_perpetual_throughput2325;

BENCHMARK_F(setup_perpetual_throughput2325, Perpetual, Prime2325, 5)
{
    run_benchmark();
}


int main(int argc, const char* argv[])
{
    srand(static_cast<uint32_t>(time(0)));

    gauge::runner::add_default_printers();
    gauge::runner::run_benchmarks(argc, argv);

    return 0;
}
