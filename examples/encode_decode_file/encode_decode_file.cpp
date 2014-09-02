// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <iostream>
#include <fstream>
#include <kodo/object/file_encoder.hpp>
// #include <kodo/object_decoder.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>
// #include <kodo/set_systematic_off.hpp>

/// @todo re-enable this unit test

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
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    uint32_t file_size = 23456;

    std::string encode_filename = "encode-file.bin";
    std::string decode_filename = "decode-file.bin";

    // Create a test file for encoding.
    std::ofstream encode_file;
    encode_file.open (encode_filename, std::ios::binary);
    std::vector<char> data_in(file_size, 'x');

    encode_file.write(data_in.data(), data_in.size());
    encode_file.close();

    using file_encoder_factory = kodo::object::file_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary> >::factory;

    // using storage_decoder = kodo::object::shallow_storage_decoder<
    //     kodo::shallow_full_rlnc_decoder<fifi::binary> >;

    file_encoder_factory encoder_factory(max_symbols, max_symbol_size);
    // storage_decoder::factory decoder_factory(max_symbols, max_symbol_size);

    // std::vector<uint8_t> data_out(object_size, '\0');


    encoder_factory.set_filename(encode_filename);

    // decoder_factory.set_filename("decode-file.bin");
    // decoder_factory.set_filesize(file_size);

    auto file_encoder = encoder_factory.build();
    // auto file_decoder = decoder_factory.build();

    std::cout << "encoder blocks = " << file_encoder->blocks() << std::endl;
    // std::cout << "decoder blocks = " << file_decoder->blocks() << std::endl;

    for (uint32_t i = 0; i < file_encoder->blocks(); ++i)
    {
        auto e = file_encoder->build(i);
        // auto d = file_decoder->build(i);

        std::vector<uint8_t> payload(e->payload_size());

        // while (!d->is_complete())
        // {
            e->encode( payload.data() );

            // Here we would send and receive the payload over a
            // network. Lets throw away some packet to simulate.
            // if (rand() % 2)
            // {
                // continue;
            // }

            // d->decode( payload.data() );
        // }
    }

    // Check we properly decoded the data
    // if (data_in == data_out)
    // {
        // std::cout << "Data decoded correctly" << std::endl;
    // }
    // else
    // {
        // std::cout << "Unexpected failure to decode "
                  // << "please file a bug report :)" << std::endl;
    // }

//     // Set the number of symbols (i.e. the generation size in RLNC
//     // terminology) and the size of a symbol in bytes
//     uint32_t max_symbols = 42;
//     uint32_t max_symbol_size = 64;

//     std::string encode_filename = "encode-file.bin";

//     // Create a test file for encoding.
//     std::ofstream encode_file;
//     encode_file.open (encode_filename, std::ios::binary);

//     uint32_t file_size = 50000;
//     std::vector<char> encode_data(file_size);
//     std::vector<char> decode_data;

//     // Just write some bytes to the file
//     for(uint32_t i = 0; i < file_size; ++i)
//     {
//         encode_data[i] = rand() % 255;
//     }
//     encode_file.write(&encode_data[0], file_size);
//     encode_file.close();

//     // Select the encoding and decoding algorithms
//     typedef kodo::full_rlnc_encoder<fifi::binary>
//         encoder_t;

//     typedef kodo::full_rlnc_decoder<fifi::binary>
//         decoder_t;

//     // Now for the encoder we use a file_encoder with the chosen
//     // encoding algorithm
//     typedef kodo::file_encoder<encoder_t>
//         file_encoder_t;

//     // For decoding we use an object_decoder with the chosen
//     // decoding algorithm
//     typedef kodo::object_decoder<decoder_t>
//         object_decoder_t;

//     // Create the encoder factory - builds the individual encoders used
//     file_encoder_t::factory encoder_factory(max_symbols, max_symbol_size);

//     // Create the actual file encoder using the encoder factory and
//     // the filename of the file to be encoded
//     file_encoder_t file_encoder(encoder_factory, encode_filename);

//     // Create the decoder factory - build the individual decoders used
//     object_decoder_t::factory decoder_factory(max_symbols, max_symbol_size);

//     // Create the object decoder using the decoder factory and the
//     // size of the file to be decoded
//     object_decoder_t object_decoder(decoder_factory, file_size);

//     // Now in the following loop we go through all the encoders
//     // needed to encode the entire file. We the build the corresponding
//     // decoder and decode the chunk immediately. In practice where
//     // encoders and decoders are on different devices e.g. connected
//     // over a network, we would have to pass also the encoder and decoder
//     // index between the source and sink to allow the correct data would
//     // passed from encoder to corresponding decoder.
//     for(uint32_t i = 0; i < file_encoder.encoders(); ++i)
//     {
//         auto encoder = file_encoder.build(i);
//         auto decoder = object_decoder.build(i);

//         // Set the encoder non-systematic
//         if(kodo::has_systematic_encoder<encoder_t>::value)
//             kodo::set_systematic_off(encoder);

//         std::vector<uint8_t> payload(encoder->payload_size());

//         while( !decoder->is_complete() )
//         {
//             // Encode a packet into the payload buffer
//             encoder->encode( &payload[0] );

//             // In practice send the payload over a network, save it to
//             // a file etc. Then when needed build and pass it to the decoder

//             // Pass that packet to the decoder
//             decoder->decode( &payload[0] );
//         }

//         std::vector<uint8_t> data_out(decoder->block_size());
//         decoder->copy_symbols(sak::storage(data_out));
//         data_out.resize(decoder->bytes_used());

//         decode_data.insert(decode_data.end(),
//                            data_out.begin(),
//                            data_out.end());
//     }

//     // Check we properly decoded the data
//     if (std::equal(decode_data.begin(),
//                    decode_data.end(), encode_data.begin()))
//     {
//         std::cout << "Data decoded correctly" << std::endl;
//     }
//     else
//     {
//         std::cout << "Unexpected failure to decode "
//                   << "please file a bug report :)" << std::endl;
//     }
}
