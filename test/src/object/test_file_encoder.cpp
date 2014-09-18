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
#include <boost/filesystem.hpp>

#include <kodo/object/file_encoder.hpp>
#include <kodo/object/file_decoder.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>

#include "../kodo_unit_test/declare_stack_factories.hpp"
#include "../kodo_unit_test/basic_run_object_stacks.hpp"

namespace kodo
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

            // Check that the decoded file now exists and has the
            // right size
            EXPECT_TRUE(boost::filesystem::is_regular_file(decode_filename));

            EXPECT_EQ(boost::filesystem::file_size(decode_filename),
                      file_size);

            // Compare the decoded data with the input data
            std::ifstream decode_file;
            decode_file.open(decode_filename, std::ios::binary);
            std::vector<char> data_out(file_size);

            decode_file.read(data_out.data(), data_out.size());
            decode_file.close();

            EXPECT_TRUE(data_in == data_out);
        }
    };

    /// This test layer make sure that decoding output file does not exist
    template<class SuperTest>
    class remove_output_file : public SuperTest
    {
    public:

        remove_output_file(uint32_t max_symbols,
                           uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            /// @todo rename the filename function to file_name
            auto file_name = SuperTest::decoder_factory().filename();

            if (boost::filesystem::is_regular_file(file_name))
            {
                boost::filesystem::remove(file_name);
            }

            SuperTest::run();
        }
    };

    /// This test layer will create a random file in the location
    /// where the file decoder will try to decode the file.
    template<class SuperTest>
    class create_random_output_file : public SuperTest
    {
    public:

        create_random_output_file(uint32_t max_symbols,
                                  uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            /// @todo rename the filename function to file_name
            auto file_name = SuperTest::decoder_factory().filename();
            auto file_size = SuperTest::decoder_factory().file_size();

            EXPECT_TRUE(file_name != "");
            EXPECT_TRUE(file_size > 0);

            // Make the random size a random value between [1:file_size*2]
            auto random_size = (rand() % (file_size * 2)) + 1;

            // Create some random data
            std::vector<char> data(random_size);
            std::generate(data.begin(), data.end(), rand);

            // Write the file
            std::ofstream random_file;
            random_file.open(file_name, std::ios::binary);
            random_file.write(data.data(), data.size());
            random_file.close();

            SuperTest::run();
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
}

// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{

    template<class ObjectEncoder, class ObjectDecoder>
    using test_file =
        kodo::setup_file<
        kodo::remove_output_file<
        kodo::basic_run_object_stacks<
        kodo::declare_stack_factories<ObjectEncoder, ObjectDecoder,
        kodo::final>>>>;

    template<class ObjectEncoder, class ObjectDecoder>
    using test_file_output_exists =
        kodo::setup_file<
        kodo::remove_output_file<
        kodo::create_random_output_file<
        kodo::basic_run_object_stacks<
        kodo::declare_stack_factories<ObjectEncoder, ObjectDecoder,
        kodo::final>>>>>;

    template<class ObjectEncoder, class ObjectDecoder>
    using test_file_random =
        kodo::setup_file<
        kodo::remove_output_file<
        kodo::random_run<
        kodo::declare_stack_factories<ObjectEncoder, ObjectDecoder,
        kodo::final>>>>;

    template<class ObjectEncoder, class ObjectDecoder>
    using test_file_duplicate_blocks =
        kodo::setup_file<
        kodo::remove_output_file<
        kodo::duplicate_blocks<
        kodo::declare_stack_factories<ObjectEncoder, ObjectDecoder,
        kodo::final>>>>;

}

/// Just run though the basic functionality of the file_encoder and
/// file_decoder and check that everything works
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

    using test = test_file<encoder, decoder>;

    test t(max_symbols, max_symbol_size);
    t.run();
}

// Tests that we decode correctly even if the file we want to decode
// into already exists
// Test that we decode correctly even if we take blocks in random order
TEST(ObjectTestFileEncoder, output_exists)
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    using encoder = kodo::object::file_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using decoder = kodo::object::file_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    using test = test_file_output_exists<encoder, decoder>;

    test t(max_symbols, max_symbol_size);
    t.run();
}

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

    using test = test_file_duplicate_blocks<encoder, decoder>;

    test t(max_symbols, max_symbol_size);
    t.run();
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

    using test = test_file_random<encoder, decoder>;

    test t(max_symbols, max_symbol_size);
    t.run();
}
