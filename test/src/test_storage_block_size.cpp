// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_storage_block_size.cpp Unit tests for the
///       storage_block_size class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/storage_block_size.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        /// Helper layer
        class dummy_layer
        {
        public:

            class factory_base
            {
            public:
                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }
            };

        public:

            template<class Factory>
            void initialize(Factory &the_factory)
            {
                (void) the_factory;
            }

        };

        // Helper stack
        class dummy_stack : public storage_block_size<dummy_layer>
        { };

    }
}

TEST(TestStorageBlockSize, api)
{
    uint32_t max_symbols = 10U;
    uint32_t max_symbol_size = 100U;

    kodo::dummy_stack::factory_base factory(max_symbols, max_symbol_size);

    EXPECT_EQ(factory.max_symbols(), max_symbols);
    EXPECT_EQ(factory.max_symbol_size(), max_symbol_size);
    EXPECT_EQ(factory.max_block_size(), max_symbols*max_symbol_size);
    EXPECT_EQ(factory.symbols(), max_symbols);
    EXPECT_EQ(factory.symbol_size(), max_symbol_size);

    kodo::dummy_stack stack;
    stack.initialize(factory);

    EXPECT_EQ(stack.symbols(), max_symbols);
    EXPECT_EQ(stack.symbol_size(), max_symbol_size);
    EXPECT_EQ(stack.block_size(), max_symbols*max_symbol_size);

    uint32_t symbols = 5U;
    uint32_t symbol_size = 99U;

    factory.set_symbols(symbols);
    factory.set_symbol_size(symbol_size);

    EXPECT_EQ(factory.symbols(), symbols);
    EXPECT_EQ(factory.symbol_size(), symbol_size);

    stack.initialize(factory);

    EXPECT_EQ(stack.symbols(), symbols);
    EXPECT_EQ(stack.symbol_size(), symbol_size);
    EXPECT_EQ(stack.block_size(), symbols*symbol_size);
}
