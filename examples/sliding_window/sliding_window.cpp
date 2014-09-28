// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <algorithm>

#include <kodo/rlnc/sliding_window_encoder.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>
#include <kodo/trace.hpp>

/// @example sliding_window.cpp
///
/// This example shows how to use sliding window encoder and decoder
/// stacks. The sliding window is special in that it does not require
/// that all symbols are available at the encoder before encoding can
/// start. In addition it uses feedback beteen the decoder and encoder
/// such that symbols that have already been received at the decoder
/// are not included in the encoding again (saving computations).

int main()
{
    srand((uint32_t)time(0));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 16;
    uint32_t symbol_size = 160;

    // Typdefs for the encoder/decoder type we wish to use
    using rlnc_encoder = kodo::sliding_window_encoder<fifi::binary8>;
    using rlnc_decoder = kodo::sliding_window_decoder<fifi::binary8>;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoders
    kodo::pool_factory<rlnc_encoder> encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    kodo::pool_factory<rlnc_decoder> decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    std::vector<uint8_t> payload(encoder->payload_size());

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    std::vector<uint8_t> data_in(encoder->block_size());

    // The sliding window codec stacks uses feedback to optimize the
    // coding. Using the following we can allocate a buffer for the
    // feedback buffer.
    std::vector<uint8_t> feedback(encoder->feedback_size());

    // Just for fun - fill the data with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Lets split the data into symbols and feed the encoder one symbol
    // at a time
    auto symbol_storage =
        sak::split_storage(sak::storage(data_in), symbol_size);

    while ( !decoder->is_complete() )
    {

        if (kodo::has_trace<rlnc_decoder>::value)
        {
            kodo::trace(decoder, std::cout);
        }

        // Randomly choose to insert a symbol
        if ((rand() % 2) && (encoder->rank() < symbols))
        {
            // For an encoder the rank specifies the number of symbols
            // it has available for encoding
            uint32_t rank = encoder->rank();

            encoder->set_symbol(rank, symbol_storage[rank]);

            std::cout << "Symbol " << rank << " added to the encoder"
                      << std::endl;
        }

        // Encode a packet into the payload buffer
        encoder->encode(payload.data());

        std::cout << "Packet encoded" << std::endl;

        // Send the data to the decoders, here we just for fun
        // simulate that we are loosing 50% of the packets
        if (rand() % 2)
        {
            std::cout << "Packet dropped on channel" << std::endl;
            continue;
        }

        std::cout << "Decoder received packet" << std::endl;

        // Packet got through - pass that packet to the decoder
        decoder->decode(payload.data());

        std::cout << "Encoder rank = " << encoder->rank() << std::endl;
        std::cout << "Decoder rank = " << decoder->rank() << std::endl;
        std::cout << "Decoder uncoded = " << decoder->symbols_uncoded()
                  << std::endl;
        std::cout << "Decoder seen = " << decoder->symbols_seen() << std::endl;

        // Transmit the feedback
        decoder->write_feedback(feedback.data());

        // Simulate loss of feedback
        if (rand() % 2)
        {
            std::cout << "Lost feedback from decoder" << std::endl;
            continue;
        }

        std::cout << "Received feedback from decoder" << std::endl;

        encoder->read_feedback(feedback.data());
    }

    // The decoder is complete, now copy the symbols from the decoder
    std::vector<uint8_t> data_out(decoder->block_size());
    decoder->copy_symbols(sak::storage(data_out));

    // Check we properly decoded the data
    if (data_out == data_in)
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }

}
