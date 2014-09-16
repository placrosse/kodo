// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_storage_block_length.cpp Unit tests for the
///       storage_block_length class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <fifi/binary8.hpp>
#include <kodo/storage_block_length.hpp>


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

            using field_type = fifi::binary8;

        public:

            class factory_base
            {
            public:
                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                uint32_t max_symbol_size() const
                {
                    return m_max_symbol_size();
                }

                uint32_t max_block_size() const
                {
                    return m_max_block_size();
                }

                uint32_t symbol_size() const
                {
                    return m_symbol_size();
                }

            public:

                stub::call<uint32_t()> m_max_symbol_size;
                stub::call<uint32_t()> m_max_block_size;
                stub::call<uint32_t()> m_symbol_size;
            };

        public:

            uint32_t symbol_size() const
            {
                return m_symbol_size();
            }

            uint32_t block_size() const
            {
                return m_block_size();
            }

            stub::call<uint32_t()> m_symbol_size;
            stub::call<uint32_t()> m_block_size;
        };

        // Helper stack
        class dummy_stack : public storage_block_length<dummy_layer>
        { };

    }
}

TEST(TestStorageBlockLength, api)
{
    uint32_t max_symbols = 10U;
    uint32_t max_symbol_size = 100U;

    kodo::dummy_stack::factory_base factory(max_symbols, max_symbol_size);

    factory.m_max_symbol_size.set_return(1U);
    factory.m_max_block_size.set_return(2U);
    factory.m_symbol_size.set_return(3U);

    kodo::dummy_stack stack;

    stack.m_symbol_size.set_return(4U);
    stack.m_block_size.set_return(5U);

    // Since we use the fifi::binary8 field the length and size will
    // be the same
    EXPECT_EQ(factory.max_symbol_length(), 1U);
    EXPECT_EQ(factory.max_block_length(), 2U);
    EXPECT_EQ(factory.symbol_length(), 3U);

    EXPECT_EQ(stack.symbol_length(), 4U);
    EXPECT_EQ(stack.block_length(), 5U);
}
