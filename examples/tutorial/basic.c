// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <ckodo/ckodo.h>
#include <stdint.h>

/// Simple example showing how to encode and decode a block
/// of memory.

int main()
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 160;

    // Here we select the coding algorithm we wish to use
    size_t algorithm = kodo_debug_full_rlnc;

    // Here we select the finite field to use common choices are
    // kodo_binary, kodo_binary8, kodo_binary16
    size_t finite_field = kodo_binary;

    kodo_factory_t* encoder_factory =
        kodo_new_encoder_factory(algorithm, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_factory_t* decoder_factory =
        kodo_new_decoder_factory(algorithm, finite_field,
                                 max_symbols, max_symbol_size);

    // If we wanted to build an encoder of decoder with a smaller number of
    // symbols or a different symbol size, then this can be adjusted using the
    // following functions:
    // kodo_factory_set_symbols(...) and kodo_factory_set_symbol_size(...)
    // We can however not exceed the maximum values which was used when building
    // the factory.

    kodo_coder_t* encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t* decoder = kodo_factory_new_decoder(decoder_factory);

    uint32_t bytes_used = 0U;
    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for(; i < block_size; ++i)
        data_in[i] = rand() % 256;

    kodo_set_symbols(encoder, data_in, block_size);

    // Most of the network coding algorithms supports a mode of operation
    // which is known as systematic coding. This basically means that
    // initially all symbols are sent once un-coded. The rational behind this
    // is that if no errors occur during the transmission we will not have
    // performed any unnecessary coding operations. An encoder will exit the
    // systematic phase automatically once all symbols have been sent un-coded
    // once.
    //
    // With Kodo we can ask an encoder whether it supports systematic encoding
    // or not using the following functions:

    if(kodo_is_systematic(encoder))
    {
        printf("Encoder systematic enabled\n");
    }
    else
    {
        printf("Encoder systematic disabled\n");
    }

    // If we do not wish to use systematic encoding, but to do full coding
    // from the beginning we can turn systematic coding off using the following
    // API:
    //
    // if(kodo_is_systematic(encoder))
    // {
    //    kodo_set_systematic_off(encoder);
    // }

    while(!kodo_is_complete(decoder))
    {
        // The encoder will use a certain amount of bytes of the payload
        // buffer. It will however never use more than payload_size, but
        // it might use less.
        printf("Encode packet\n");
        bytes_used = kodo_encode(encoder, payload);
        kodo_decode(decoder, payload);

        if(kodo_has_print_decoder_state(decoder))
            kodo_print_decoder_state(decoder);

    }

    kodo_copy_symbols(decoder, data_out, block_size);

    if(memcmp(data_in, data_out, block_size) == 0)
    {
        printf("Data decoded correctly\n");
    }
    else
    {
        printf("Unexpected failure to decode please file a bug report :)\n");
    }

    free(data_in);
    free(data_out);
    free(payload);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);

    return 0;
}

