// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <ctime>
#include <stack>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <gauge/console_printer.hpp>
#include <gauge/csv_printer.hpp>
#include <gauge/gauge.hpp>
#include <gauge/python_printer.hpp>

#include <tables/table.hpp>

#include <kodo/set_systematic_off.hpp>

#include "codes.hpp"

template<class Encoder, class Decoder>
class operations_benchmark : public gauge::benchmark
{
public:

    typedef typename Encoder::factory encoder_factory;
    typedef typename Encoder::factory::pointer encoder_ptr;

    typedef typename Decoder::factory decoder_factory;
    typedef typename Decoder::factory::pointer decoder_ptr;

    /// Starts a measurement and saves the counter
    void start()
    {
        m_encoder->reset_operations_counter();
        m_decoder->reset_operations_counter();
    }

    /// Stops a measurement and saves the counter
    void stop()
    {
        gauge::config_set cs = get_current_configuration();

        std::string type = cs.get_value<std::string>("type");

        if(type == "encoder")
        {
            m_counter = m_encoder->get_operations_counter();
        }
        else if(type == "decoder")
        {
            m_counter = m_decoder->get_operations_counter();
        }
        else
        {
            assert(0);
        }
    }

    void store_run(tables::table& results)
    {
        if(!results.has_column("dest[i] = dest[i] + src[i]"))
            results.add_column("dest[i] = dest[i] + src[i]");

        results.set_value("dest[i] = dest[i] + src[i]", m_counter.m_add);

        if(!results.has_column("dest[i] = dest[i] - src[i]"))
            results.add_column("dest[i] = dest[i] - src[i]");

        results.set_value("dest[i] = dest[i] - src[i]", m_counter.m_subtract);

        if(!results.has_column("dest[i] = dest[i] * constant"))
            results.add_column("dest[i] = dest[i] * constant");

        results.set_value("dest[i] = dest[i] * constant", m_counter.m_multiply);

        if(!results.has_column("dest[i] = dest[i] + (constant * src[i])"))
            results.add_column("dest[i] = dest[i] + (constant * src[i])");

        results.set_value("dest[i] = dest[i] + (constant * src[i])",
            m_counter.m_multiply_add);

        if(!results.has_column("dest[i] = dest[i] - (constant * src[i])"))
            results.add_column("dest[i] = dest[i] - (constant * src[i])");

        results.set_value("dest[i] = dest[i] - (constant * src[i])",
            m_counter.m_multiply_subtract);

        if(!results.has_column("invert(value)"))
            results.add_column("invert(value)");

        results.set_value("invert(value)", m_counter.m_invert);
    }


    /// Prepares the measurement for every run
    void setup()
    {
        gauge::config_set cs = get_current_configuration();

        uint32_t symbols = cs.get_value<uint32_t>("symbols");
        uint32_t symbol_size = cs.get_value<uint32_t>("symbol_size");

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

        m_payload_buffer.resize(m_encoder->payload_size(), 0);
        m_encoded_data.resize(m_encoder->block_size(), 'x');

        m_encoder->set_symbols(sak::storage(m_encoded_data));
    }

    /// Where the actual measurement is performed
    void run()
    {

        // Check with the result memory whether we already have
        // results for this configuration
        gauge::config_set cs = get_current_configuration();

        // We switch any systematic operations off so we code
        // symbols from the beginning
        if(kodo::has_systematic_encoder<Encoder>::value)
            kodo::set_systematic_off(m_encoder);

        RUN{

            while( !m_decoder->is_complete() )
            {
                // Encode a packet into the payload buffer
                m_encoder->encode( &m_payload_buffer[0] );

                if(rand() % 2)
                {
                    // Pass that packet to the decoder
                    m_decoder->decode( &m_payload_buffer[0] );
                }
            }
        }
    }

    void get_options(gauge::po::variables_map& options)
    {
        auto symbols = options["symbols"].as<std::vector<uint32_t> >();
        auto symbol_size = options["symbol_size"].as<std::vector<uint32_t> >();
        auto types = options["type"].as<std::vector<std::string> >();

        assert(symbols.size() > 0);
        assert(symbol_size.size() > 0);
        assert(types.size() > 0);

        for(uint32_t i = 0; i < symbols.size(); ++i)
        {
            for(uint32_t j = 0; j < symbol_size.size(); ++j)
            {
                for(uint32_t u = 0; u < types.size(); ++u)
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

    /// The unit we measure in
    std::string unit_text() const
    { return "operations per symbol"; }

protected:

    /// The decoder factory
    std::shared_ptr<decoder_factory> m_decoder_factory;

    /// The encoder factory
    std::shared_ptr<encoder_factory> m_encoder_factory;

    /// The decoder
    decoder_ptr m_decoder;

    /// The Encoder
    encoder_ptr m_encoder;

    /// The payload buffer
    std::vector<uint8_t> m_payload_buffer;

    /// The payload buffer
    std::vector<uint8_t> m_encoded_data;

    /// The counter containing the measurement results
    kodo::operations_counter m_counter;

};

/// Using this macro we may specify options. For specifying options
/// we use the boost program options library. So you may additional
/// details on how to do it in the manual for that library.
BENCHMARK_OPTION(count_operations_options)
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


typedef operations_benchmark<
    kodo::full_rlnc_encoder_count<fifi::binary>,
    kodo::full_rlnc_decoder_count<fifi::binary> > setup_rlnc_count;

BENCHMARK_F(setup_rlnc_count, RLNC, Binary, 5)
{
    run();
}

typedef operations_benchmark<
    kodo::full_rlnc_encoder_count<fifi::binary8>,
    kodo::full_rlnc_decoder_count<fifi::binary8> > setup_rlnc_count8;

BENCHMARK_F(setup_rlnc_count8, RLNC, Binary8, 5)
{
    run();
}

typedef operations_benchmark<
    kodo::full_rlnc_encoder_count<fifi::binary16>,
    kodo::full_rlnc_decoder_count<fifi::binary16> > setup_rlnc_count16;

BENCHMARK_F(setup_rlnc_count16, RLNC, Binary16, 5)
{
    run();
}

typedef operations_benchmark<
    kodo::full_rlnc_encoder_count<fifi::binary>,
    kodo::full_delayed_rlnc_decoder_count<fifi::binary> >
    setup_delayed_rlnc_count;

BENCHMARK_F(setup_delayed_rlnc_count, DelayedRLNC, Binary, 5)
{
    run();
}

typedef operations_benchmark<
   kodo::full_rlnc_encoder_count<fifi::binary8>,
   kodo::full_delayed_rlnc_decoder_count<fifi::binary8> >
   setup_delayed_rlnc_count8;

BENCHMARK_F(setup_delayed_rlnc_count8, DelayedRLNC, Binary8, 5)
{
   run();
}

typedef operations_benchmark<
   kodo::full_rlnc_encoder_count<fifi::binary16>,
   kodo::full_delayed_rlnc_decoder_count<fifi::binary16> >
   setup_delayed_rlnc_count16;

BENCHMARK_F(setup_delayed_rlnc_count16, DelayedRLNC, Binary16, 5)
{
   run();
}


int main(int argc, const char* argv[])
{

    srand(static_cast<uint32_t>(time(0)));

    gauge::runner::add_default_printers();
    gauge::runner::run_benchmarks(argc, argv);

    return 0;
}
