// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo/file_encoder.hpp>
#include <kodo/shallow_storage_decoder.hpp>
#include <kodo/storage_encoder.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/partial_shallow_symbol_storage.hpp>

#include <vector>

/// @example encode_decode_storage.cpp
///
/// Often we want to encode / decode data that exceed a single
/// encoding/decoding block. In this case we need to "chop" up
/// the data into manageable chunks and then encode and decode
/// each chuck separately. This examples shows how to use the
/// storage encoder and decoder in Kodo.
///
/// Both the encoder and decoder uses a shallow storage which means
/// that they operate directly on the memory provided.

int main()
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;
    uint32_t object_size = 23456;

    using storage_encoder = kodo::storage_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using storage_decoder = kodo::shallow_storage_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    storage_encoder::factory encoder_factory(max_symbols, max_symbol_size);
    storage_decoder::factory decoder_factory(max_symbols, max_symbol_size);

    // The storage needed for all decoders
    uint32_t total_block_size = decoder_factory.total_block_size(object_size);

    std::vector<uint8_t> data_out(total_block_size, '\0');
    std::vector<uint8_t> data_in(object_size, 'x');

    storage_encoder encoder(encoder_factory, sak::storage(data_in));

    storage_decoder decoder(decoder_factory, object_size,
        sak::storage(data_out));

    for (uint32_t i = 0; i < encoder.encoders(); ++i)
    {
        auto e = encoder.build(i);
        auto d = decoder.build(i);

        std::vector<uint8_t> payload(e->payload_size());

        while (!d->is_complete())
        {
            e->encode(payload.data());

            // Here we would send and receive the payload over a
            // network. Lets throw away some packet to simulate.
            if (rand() % 2)
            {
                continue;
            }

            d->decode(payload.data());
        }
    }

    // Resize the output buffer to contain only the object data
    data_out.resize(object_size);

    // Check we properly decoded the data
    if (data_in == data_out)
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }
}
