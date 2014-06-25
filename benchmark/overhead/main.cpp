// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <ctime>

#include <boost/make_shared.hpp>

#include <gauge/gauge.hpp>
#include <gauge/console_printer.hpp>
#include <gauge/python_printer.hpp>
#include <gauge/csv_printer.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/rlnc/seed_rlnc_codes.hpp>
#include <kodo/reed_solomon/reed_solomon_codes.hpp>
#include <kodo/has_deep_symbol_storage.hpp>

#include <tables/table.hpp>

#include "codes.hpp"

/// A test block represents an encoder and decoder pair
template<class Encoder, class Decoder>
struct overhead_benchmark : public gauge::benchmark
{

    typedef typename Encoder::factory encoder_factory;
    typedef typename Encoder::factory::pointer encoder_ptr;

    typedef typename Decoder::factory decoder_factory;
    typedef typename Decoder::factory::pointer decoder_ptr;

    static_assert(kodo::has_deep_symbol_storage<Decoder>::value,
                  "The decoder should bring its own memory");

    void start()
    { }

    void stop()
    { }

    void store_run(tables::table& results)
    {
        assert(m_encoder->block_size() ==
               m_decoder->block_size());

        assert(m_bytes_used > 0);
        assert(m_bytes_used >= m_encoder->block_size());

        if(!results.has_column("coded"))
            results.add_column("coded");
        results.set_value("coded", m_encoder->block_size());

        if(!results.has_column("used"))
            results.add_column("used");
        results.set_value("used", m_bytes_used);
    }

    std::string unit_text() const
    {
        return "byte";
    }

    void get_options(gauge::po::variables_map& options)
    {
        auto symbols = options["symbols"].as<std::vector<uint32_t> >();
        auto symbol_size = options["symbol_size"].as<std::vector<uint32_t> >();

        assert(symbols.size() > 0);
        assert(symbol_size.size() > 0);

        m_max_symbols = *std::max_element(symbols.begin(),
                                          symbols.end());

        m_max_symbol_size = *std::max_element(symbol_size.begin(),
                                              symbol_size.end());

        // Make the factories fit perfectly otherwise there seems to
        // be problems with memory access i.e. when using a factory
        // with max symbols 1024 with a symbols 16
        m_decoder_factory = std::make_shared<decoder_factory>(
            m_max_symbols, m_max_symbol_size);

        m_encoder_factory = std::make_shared<encoder_factory>(
            m_max_symbols, m_max_symbol_size);

        for(uint32_t i = 0; i < symbols.size(); ++i)
        {
            for(uint32_t j = 0; j < symbol_size.size(); ++j)
            {
                gauge::config_set cs;
                cs.set_value<uint32_t>("symbols", symbols[i]);
                cs.set_value<uint32_t>("symbol_size", symbol_size[j]);
                add_configuration(cs);
            }
        }
    }

    void setup()
    {
        gauge::config_set cs = get_current_configuration();

        uint32_t symbols = cs.get_value<uint32_t>("symbols");
        uint32_t symbol_size = cs.get_value<uint32_t>("symbol_size");

        m_decoder_factory->set_symbols(symbols);
        m_decoder_factory->set_symbol_size(symbol_size);

        m_encoder_factory->set_symbols(symbols);
        m_encoder_factory->set_symbol_size(symbol_size);

        m_encoder = m_encoder_factory->build();
        m_decoder = m_decoder_factory->build();

        // Prepare the data to be encoded
        m_encoded_data.resize(m_encoder->block_size());

        for(uint8_t &e : m_encoded_data)
        {
            e = rand() % 256;
        }

        m_encoder->set_symbols(sak::storage(m_encoded_data));

        m_bytes_used = 0;
    }

    /// Run the benchmark
    void run_benchmark()
    {
        assert(m_bytes_used == 0);
        assert(m_encoder);
        assert(m_decoder);

        std::vector<uint8_t> payload(m_encoder->payload_size());

        // The clock is running
        RUN{

            while(!m_decoder->is_complete())
            {
                m_bytes_used += m_encoder->encode(&payload[0]);
                m_decoder->decode(&payload[0]);
            }
        }
    }

protected:

    /// The maximum number of symbols
    uint32_t m_max_symbols;

    /// The maximum symbol size
    uint32_t m_max_symbol_size;

    /// The decoder factory
    std::shared_ptr<decoder_factory> m_decoder_factory;

    /// The encoder factory
    std::shared_ptr<encoder_factory> m_encoder_factory;

    /// The encoder to use
    encoder_ptr m_encoder;

    /// The encoder to use
    decoder_ptr m_decoder;

    /// The data to encode
    std::vector<uint8_t> m_encoded_data;

    /// The number of bytes used
    uint32_t m_bytes_used;

};

/// Using this macro we may specify options. For specifying options
/// we use the boost program options library. So you may additional
/// details on how to do it in the manual for that library.
BENCHMARK_OPTION(overhead_options)
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
    symbol_size.push_back(1500);

    auto default_symbol_size =
        gauge::po::value<std::vector<uint32_t> >()->default_value(
            symbol_size, "")->multitoken();

    std::vector<double> erasure;
    erasure.push_back(0.5);

    auto default_erasure =
        gauge::po::value<std::vector<double> >()->default_value(
            erasure, "")->multitoken();

    options.add_options()
        ("symbols", default_symbols, "Set the number of symbols");

    options.add_options()
        ("symbol_size", default_symbol_size, "Set the symbol size in bytes");

    options.add_options()
        ("erasure", default_erasure,
         "Set the symbol erasure probability");

    gauge::runner::instance().register_options(options);
}


typedef overhead_benchmark<
    kodo::full_rlnc_encoder<fifi::binary>,
    kodo::full_rlnc_decoder<fifi::binary> > setup_rlnc_overhead;

BENCHMARK_F(setup_rlnc_overhead, FullRLNC, Binary, 5)
{
    run_benchmark();
}

typedef overhead_benchmark<
    kodo::full_rlnc_encoder<fifi::binary8>,
    kodo::full_rlnc_decoder<fifi::binary8> > setup_rlnc_overhead8;

BENCHMARK_F(setup_rlnc_overhead8, FullRLNC, Binary8, 5)
{
    run_benchmark();
}

typedef overhead_benchmark<
    kodo::full_rlnc_encoder<fifi::binary16>,
    kodo::full_rlnc_decoder<fifi::binary16> > setup_rlnc_overhead16;

BENCHMARK_F(setup_rlnc_overhead16, FullRLNC, Binary16, 5)
{
    run_benchmark();
}

typedef overhead_benchmark<
   kodo::full_rlnc_encoder<fifi::binary>,
   kodo::full_delayed_rlnc_decoder<fifi::binary> >
   setup_delayed_rlnc_overhead;

BENCHMARK_F(setup_delayed_rlnc_overhead, FullDelayedRLNC, Binary, 5)
{
   run_benchmark();
}

typedef overhead_benchmark<
   kodo::full_rlnc_encoder<fifi::binary8>,
   kodo::full_delayed_rlnc_decoder<fifi::binary8> >
   setup_delayed_rlnc_overhead8;

BENCHMARK_F(setup_delayed_rlnc_overhead8, FullDelayedRLNC, Binary8, 5)
{
   run_benchmark();
}

typedef overhead_benchmark<
   kodo::full_rlnc_encoder<fifi::binary16>,
   kodo::full_delayed_rlnc_decoder<fifi::binary16> >
   setup_delayed_rlnc_overhead16;

BENCHMARK_F(setup_delayed_rlnc_overhead16, FullDelayedRLNC, Binary16, 5)
{
   run_benchmark();
}

//------------------------------------------------------------------
// Unsystematic encoder and decoder
//------------------------------------------------------------------


typedef overhead_benchmark<
   kodo::full_rlnc_encoder_unsystematic<fifi::binary>,
   kodo::full_rlnc_decoder_unsystematic<fifi::binary> >
   setup_full_rlnc_unsystematic;

BENCHMARK_F(setup_full_rlnc_unsystematic, FullRLNCUnsystematic, Binary, 5)
{
   run_benchmark();
}

typedef overhead_benchmark<
   kodo::full_rlnc_encoder_unsystematic<fifi::binary8>,
   kodo::full_rlnc_decoder_unsystematic<fifi::binary8> >
   setup_full_rlnc_unsystematic8;

BENCHMARK_F(setup_full_rlnc_unsystematic8, FullRLNCUnsystematic, Binary8, 5)
{
   run_benchmark();
}

typedef overhead_benchmark<
   kodo::full_rlnc_encoder_unsystematic<fifi::binary16>,
   kodo::full_rlnc_decoder_unsystematic<fifi::binary16> >
   setup_full_rlnc_unsystematic16;

BENCHMARK_F(setup_full_rlnc_unsystematic16, FullRLNCUnsystematic, Binary16, 5)
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
