// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_storage_encoder.cpp Unit tests for the storage_encoder
///       and storage_decoder class

#include <gtest/gtest.h>

#include <kodo/object/storage_encoder.hpp>
#include <kodo/object/storage_decoder.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>

#include "../kodo_unit_test/declare_stack_factories.hpp"
#include "../kodo_unit_test/basic_run_object_stacks.hpp"
#include "../kodo_unit_test/random_run_object_stacks.hpp"
#include "../kodo_unit_test/partial_decode_object_stacks.hpp"
#include "../kodo_unit_test/final_test.hpp"
#include "../kodo_unit_test/basic_api_test_helper.hpp"

namespace kodo
{
    /// Initialize the object_encoder and object_decoder with the
    /// memory to be used as input and output
    template<class SuperTest>
    class setup_storage : public SuperTest
    {
    public:

        setup_storage(uint32_t max_symbols, uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            uint32_t memory_size = 23456;

            std::vector<char> data_in(memory_size);
            std::vector<char> data_out(memory_size);

            std::generate(data_in.begin(), data_in.end(), rand);
            std::generate(data_out.begin(), data_out.end(), rand);

            SuperTest::encoder_factory().set_storage(sak::storage(data_in));
            SuperTest::decoder_factory().set_storage(sak::storage(data_out));

            SuperTest::run();

            EXPECT_TRUE(data_in == data_out);
        }
    };
}

// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    template<class ObjectEncoder, class ObjectDecoder>
    using test_storage =
        kodo::setup_storage<
        kodo::basic_run_object_stacks<
        kodo::declare_stack_factories<ObjectEncoder, ObjectDecoder,
        kodo::final_test>>>;

    template<class ObjectEncoder, class ObjectDecoder>
    using test_storage_random =
        kodo::setup_storage<
        kodo::random_run_object_stacks<
        kodo::declare_stack_factories<ObjectEncoder, ObjectDecoder,
        kodo::final_test>>>;

    template<class ObjectEncoder, class ObjectDecoder>
    using test_storage_duplicate_blocks =
        kodo::setup_storage<
        kodo::partial_decode_object_stacks<
        kodo::declare_stack_factories<ObjectEncoder, ObjectDecoder,
        kodo::final_test>>>;
}

void run_test_storage(uint32_t max_symbols, uint32_t max_symbol_size)
{
    using encoder = kodo::object::storage_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using decoder = kodo::object::storage_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    using test = test_storage<encoder, decoder>;

    test t(max_symbols, max_symbol_size);
    t.run();
}

/// Just run though the basic functionality of the storage_encoder and
/// storage_decoder and check that everything works
TEST(ObjectTestStorageEncoder, api)
{
    run_test_storage(42, 64);
    run_test_storage(1, 64);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    run_test_storage(symbols, symbol_size);
}

// Tests that we decode correctly even if we build the same decoder twice
TEST(ObjectTestStorageEncoder, duplicate_blocks)
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    using encoder = kodo::object::storage_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using decoder = kodo::object::storage_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    using test = test_storage_duplicate_blocks<encoder, decoder>;

    test t(max_symbols, max_symbol_size);
    t.run();
}

// Test that we decode correctly even if we take blocks in random order
TEST(ObjectTestStorageEncoder, random_blocks)
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    using encoder = kodo::object::storage_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>>;

    using decoder = kodo::object::storage_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>>;

    using test = test_storage_random<encoder, decoder>;

    test t(max_symbols, max_symbol_size);
    t.run();
}
