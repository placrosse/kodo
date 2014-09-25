// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/trace.hpp>

#include <ctime>
#include <set>
#include <string>
#include <vector>

/// @example use_trace_layers.cpp
///
/// Simple example showing how to use some of the trace layers defined
/// in Kodo.

int main()
{
    // Seed rand
    srand(static_cast<uint32_t>(time(0)));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 8;
    uint32_t symbol_size = 33;

    // Typdefs for the encoder/decoder type we wish to use
    using  rlnc_encoder =
        kodo::full_rlnc_encoder<fifi::binary8, kodo::disable_trace>;

    using rlnc_decoder =
        kodo::full_rlnc_decoder<fifi::binary8, kodo::enable_trace>;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoders
    rlnc_encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    rlnc_decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    std::vector<uint8_t> payload(encoder->payload_size());

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    std::vector<uint8_t> data_in(encoder->block_size());

    // Just for fun - fill the data with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    encoder->set_symbols(sak::storage(data_in));

    while (!decoder->is_complete())
    {
        // Encode a packet into the payload buffer
        encoder->encode(payload.data());

        if (kodo::has_trace<rlnc_encoder>::value)
        {
            std::cout << "Trace encoder:" << std::endl;
            kodo::trace(encoder, std::cout);
        }

        // Here we "simulate" a packet loss of approximately 50%
        // by dropping half of the encoded packets.
        // When running this example you will notice that the initial
        // symbols are received systematically (i.e. uncoded). After
        // sending all symbols once uncoded, the encoder will switch
        // to full coding, in which case you will see the full encoding
        // vectors being sent and received.
        if (rand() % 2)
            continue;

        // Pass that packet to the decoder
        decoder->decode(payload.data());

        if (kodo::has_trace<rlnc_decoder>::value)
        {
            auto filter = [](const std::string& zone)
            {
                std::set<std::string> filters =
                    {"decoder_state", "input_symbol_coefficients"};

                return filters.count(zone);
            };

            std::cout << "Trace decoder:" << std::endl;


            // Try to run without a filter to see the full amount of
            // output produced by the trace function. You can then
            // modify the filter to only view the information you are
            // interested in.

            kodo::trace(decoder, std::cout, filter);
            //kodo::trace(decoder, std::cout);
        }
    }

    // The decoder is complete, now copy the symbols from the decoder
    std::vector<uint8_t> data_out(decoder->block_size());
    decoder->copy_symbols(sak::storage(data_out));

    // Check we properly decoded the data
    if (std::equal(data_out.begin(), data_out.end(), data_in.begin()))
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }
}
