// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_file_encoder.cpp Unit tests for the
///       file_encoder and file_decoder class

#include <fstream>

#include <gtest/gtest.h>

#include <kodo/object/file_encoder.hpp>
#include <kodo/object/file_decoder.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>

namespace kodo
{
// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    // The encoder we will use

}
}

template<class SuperTest>
class setup_file : public SuperTest
{
public:

    void run()
    {
        uint32_t file_size = 23456;

        std::string encode_filename = "encode-file.bin";
        std::string decode_filename = "decode-file.bin";

        SuperTest::encoder_factory().set_filename(encode_filename);
        SuperTest::decoder_factory().set_filename(decode_filename);

        // Create a test file for encoding.
        std::ofstream encode_file;
        encode_file.open (encode_filename, std::ios::binary);
        std::vector<char> data_in(file_size, 'x');

        encode_file.write(data_in.data(), data_in.size());
        encode_file.close();

        SuperTest::run();

        // Read back the decoded data
        std::ifstream decode_file;
        decode_file.open (decode_filename, std::ios::binary);
        std::vector<char> data_out(file_size);

        decode_file.read(data_out.data(), data_out.size());
        decode_file.close();

        EXPECT_TRUE(data_in == data_out);
   }
};


template<class ObjectEncoder, class ObjectDecoder, class SuperTest>
class object_stacks : public SuperTest
{
public:

    // using file_encoder_factory = kodo::object::file_encoder<
    //     kodo::shallow_full_rlnc_encoder<fifi::binary>>::factory;

    // using file_decoder_factory = kodo::object::file_decoder<
    //     kodo::shallow_full_rlnc_decoder<fifi::binary>>::factory;

    // file_encoder_factory encoder_factory(max_symbols, max_symbol_size);
    // file_decoder_factory decoder_factory(max_symbols, max_symbol_size);


};

TEST(ObjectTestFileEncoder, api)
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

    // Actual encoding/decoding of the file

    using file_encoder_factory = kodo::object::file_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>::factory;

    using file_decoder_factory = kodo::object::file_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>::factory;

    file_encoder_factory encoder_factory(max_symbols, max_symbol_size);
    file_decoder_factory decoder_factory(max_symbols, max_symbol_size);

    encoder_factory.set_filename(encode_filename);

    decoder_factory.set_filename(decode_filename);
    decoder_factory.set_file_size(file_size);

    auto file_encoder = encoder_factory.build();
    auto file_decoder = decoder_factory.build();

    for (uint32_t i = 0; i < file_encoder->blocks(); ++i)
    {
        auto e = file_encoder->build(i);
        auto d = file_decoder->build(i);

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

    // Read back the decoded data
    std::ifstream decode_file;
    decode_file.open (decode_filename, std::ios::binary);
    std::vector<char> data_out(file_size);

    decode_file.read(data_out.data(), data_out.size());
    decode_file.close();

    EXPECT_TRUE(data_in == data_out);
}
