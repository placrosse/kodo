// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo/rlnc/on_the_fly_codes.hpp>

/// @example encode_on_the_fly.cpp
///
/// This example shows how to use a storage aware encoder which will
/// allow you to encode from a block before all symbols have been
/// specified. This can be useful in cases where the symbols that
/// should be encoded are produced on-the-fly.

int main()
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 42;
    uint32_t symbol_size = 160;

    // Typdefs for the encoder/decoder type we wish to use
    typedef kodo::on_the_fly_encoder<fifi::binary8> rlnc_encoder;
    typedef kodo::on_the_fly_decoder<fifi::binary8> rlnc_decoder;

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
    for(auto &e: data_in)
        e = rand() % 256;

    // Lets split the data into symbols and feed the encoder one symbol
    // at a time
    auto symbol_storage =
        sak::split_storage(sak::storage(data_in), symbol_size);

    while( !decoder->is_complete() )
    {
        // Encode a packet into the payload buffer
        encoder->encode( &payload[0] );

        // Send the data to the decoders, here we just for fun
        // simulate that we are loosing 50% of the packets
        if((rand() % 2) == 0)
           continue;

        // Packet got through - pass that packet to the decoder
        decoder->decode( &payload[0] );

        // Randomly choose to insert a symbol
        if((rand() % 2) == 0 && encoder->rank() < symbols)
        {
            // For an encoder the rank specifies the number of symbols
            // it has available for encoding
            uint32_t rank = encoder->rank();

            encoder->set_symbol(rank, symbol_storage[rank]);
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
