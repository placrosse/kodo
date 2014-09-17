// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_file_encoder.cpp Unit tests for the
///       file_encoder and file_decoder class

#include <fstream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>

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
    template<class SuperTest>
    class setup_file : public SuperTest
    {
    public:

        setup_file(uint32_t max_symbols, uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            uint32_t file_size = 23456;

            std::string encode_filename = "encode-file.bin";
            std::string decode_filename = "decode-file.bin";

            SuperTest::encoder_factory().set_filename(encode_filename);
            SuperTest::decoder_factory().set_filename(decode_filename);
            SuperTest::decoder_factory().set_file_size(file_size);

            // Create a test file for encoding.
            std::ofstream encode_file;
            encode_file.open(encode_filename, std::ios::binary);
            std::vector<char> data_in(file_size, 'x');

            encode_file.write(data_in.data(), data_in.size());
            encode_file.close();

            SuperTest::run();

            // Read back the decoded data
            std::ifstream decode_file;
            decode_file.open(decode_filename, std::ios::binary);
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

        using object_encoder_type = ObjectEncoder;
        using object_decoder_type = ObjectDecoder;

        using object_encoder_factory_type =
            typename object_encoder_type::factory;

        using object_decoder_factory_type =
            typename object_decoder_type::factory;

    public:

        object_stacks(uint32_t max_symbols, uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size),
              m_encoder_factory(max_symbols, max_symbol_size),
              m_decoder_factory(max_symbols, max_symbol_size)
        { }

        object_encoder_factory_type& encoder_factory()
        {
            return m_encoder_factory;
        }

        object_decoder_factory_type& decoder_factory()
        {
            return m_decoder_factory;
        }

    private:

        object_encoder_factory_type m_encoder_factory;
        object_decoder_factory_type m_decoder_factory;
    };

    template<class SuperTest>
    class basic_run : public SuperTest
    {
    public:

        basic_run(uint32_t max_symbols, uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {

            auto file_encoder = SuperTest::encoder_factory().build();
            auto file_decoder = SuperTest::decoder_factory().build();

            EXPECT_TRUE(file_encoder->blocks() > 0);
            EXPECT_EQ(file_encoder->blocks(), file_decoder->blocks());

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
        }
    };


    template<class SuperTest>
    class random_run : public SuperTest
    {
    public:

        random_run(uint32_t max_symbols, uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            auto file_encoder = SuperTest::encoder_factory().build();
            auto file_decoder = SuperTest::decoder_factory().build();

            EXPECT_TRUE(file_encoder->blocks() > 0);
            EXPECT_EQ(file_encoder->blocks(), file_decoder->blocks());

            // Generate the sequence of block ids which we will go through
            std::vector<uint32_t> blocks(file_encoder->blocks());
            std::iota(blocks.begin(), blocks.end(), 0U);

            // Shuffle the block ids
            auto time = std::chrono::system_clock::now();
            auto seed = time.time_since_epoch().count();

            std::shuffle(blocks.begin(), blocks.end(),
                         std::default_random_engine(seed));

            for (uint32_t i = 0; i < blocks.size(); ++i)
            {
                auto id = blocks[i];

                auto e = file_encoder->build(id);
                auto d = file_decoder->build(id);

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
        }
    };

    template<class SuperTest>
    class duplicate_blocks : public SuperTest
    {
    public:

        duplicate_blocks(uint32_t max_symbols, uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            auto file_encoder = SuperTest::encoder_factory().build();
            auto file_decoder = SuperTest::decoder_factory().build();

            EXPECT_TRUE(file_encoder->blocks() > 0);
            EXPECT_EQ(file_encoder->blocks(), file_decoder->blocks());

            // In the first loop we decode until we just need one more
            // symbol to decode then we move on.
            for (uint32_t i = 0; i < file_encoder->blocks(); ++i)
            {
                auto e = file_encoder->build(i);
                auto d = file_decoder->build(i);

                std::vector<uint8_t> payload(e->payload_size());

                while (d->rank() < d->symbols() - 1)
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

            // Run the encoding/decoding loop again, by this we will
            // build the same encoders and decoders as in the previous
            // loop
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


        }
    };


    class final
    {
    public:
        final(uint32_t max_symbols, uint32_t max_symbol_size)
        {
            (void) max_symbols;
            (void) max_symbol_size;
        }
    };

    template<class ObjectEncoder, class ObjectDecoder>
    using test_file =
        setup_file<
        basic_run<
        object_stacks<ObjectEncoder, ObjectDecoder,
        final>>>;

    template<class ObjectEncoder, class ObjectDecoder>
    using test_file_random =
        setup_file<
        random_run<
        object_stacks<ObjectEncoder, ObjectDecoder,
        final>>>;

    template<class ObjectEncoder, class ObjectDecoder>
    using test_file_duplicate_blocks =
        setup_file<
        duplicate_blocks<
        object_stacks<ObjectEncoder, ObjectDecoder,
        final>>>;

}
}

TEST(ObjectTestFileEncoder, api)
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    using encoder = kodo::object::file_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using decoder = kodo::object::file_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    kodo::test_file<encoder, decoder> test(max_symbols, max_symbol_size);
    test.run();
}

// Tests that we decode correctly even if the file we want to decode
// into already exists


// Tests that we decode correctly even if we build the same decoder twice
TEST(ObjectTestFileEncoder, duplicate_blocks)
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    using encoder = kodo::object::file_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using decoder = kodo::object::file_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    kodo::test_file_duplicate_blocks<encoder, decoder> test(max_symbols, max_symbol_size);
    test.run();
}

// Test that we decode correctly even if we take blocks in random order
TEST(ObjectTestFileEncoder, random_blocks)
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    using encoder = kodo::object::file_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using decoder = kodo::object::file_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    kodo::test_file_random<encoder, decoder> test(max_symbols, max_symbol_size);
    test.run();
}
