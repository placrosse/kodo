// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <iostream>
#include <fstream>

//! [0]
#include <kodo/object/file_encoder.hpp>
#include <kodo/object/file_decoder.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>
//! [1]

#include <string>
#include <vector>

/// @example encode_decode_file.cpp
///
/// Often we want to encode / decode data that exceed a single
/// encoding/decoding block. In this case we need to "chop" up
/// the data into manageable chunks and then encode and decode
/// each chuck separately. This examples shows how to use the
/// file encoder in Kodo. The file encoder operates directly on
/// a file in the file-system. For decoding we use an object decoder
/// which decodes data to memory, but which is compatible with
/// file encoder.

int main()
{
    //! [2]
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    uint32_t file_size = 23456;

    std::string encode_filename = "encode-file.bin";
    std::string decode_filename = "decode-file.bin";

    using file_encoder = kodo::object::file_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using file_decoder = kodo::object::file_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;
    //! [3]

    //! [4]
    // Create a test file for encoding.
    std::ofstream encode_file;

    encode_file.open (encode_filename, std::ios::binary);
    std::vector<char> data_in(file_size, 'x');

    encode_file.write(data_in.data(), data_in.size());
    encode_file.close();
    //! [5]

    //! [6]
    // Actual encoding/decoding of the file
    file_encoder::factory encoder_factory(max_symbols, max_symbol_size);
    file_decoder::factory decoder_factory(max_symbols, max_symbol_size);

    encoder_factory.set_filename(encode_filename);

    decoder_factory.set_filename(decode_filename);
    decoder_factory.set_file_size(file_size);

    auto encoder = encoder_factory.build();
    auto decoder = decoder_factory.build();

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
            e->encode( payload.data() );

            // Here we would send and receive the payload over a
            // network. Lets throw away some packet to simulate.
            if (rand() % 2)
            {
                continue;
            }

            d->decode( payload.data() );
        }
    }
    //! [9]
}
