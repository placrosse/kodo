// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstring>
#include <utility>

//! [0]
#include <kodo/object/storage_decoder.hpp>
#include <kodo/object/storage_encoder.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>
//! [1]

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
    //! [2]
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    uint32_t object_size = 23456;
    //! [3]

    //! [4]
    using storage_encoder = kodo::object::storage_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using storage_decoder = kodo::object::storage_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;
    //! [5]

    //! [6]
    storage_encoder::factory encoder_factory(max_symbols, max_symbol_size);
    storage_decoder::factory decoder_factory(max_symbols, max_symbol_size);

    std::vector<uint8_t> data_out(object_size, '\0');
    std::vector<uint8_t> data_in(object_size, 'x');

    encoder_factory.set_storage(sak::storage(data_in));
    decoder_factory.set_storage(sak::storage(data_out));

    auto encoder = encoder_factory.build();
    auto decoder = decoder_factory.build();

    std::cout << "object_size = " << object_size << std::endl;
    std::cout << "encoder blocks = " << encoder->blocks() << std::endl;
    std::cout << "decoder blocks = " << decoder->blocks() << std::endl;
    //! [7]

    //! [8]
    for (uint32_t i = 0; i < encoder->blocks(); ++i)
    {
        auto e = encoder->build(i);
        auto d = decoder->build(i);

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
    //! [9]
}
