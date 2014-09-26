// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_partitioning.cpp Unit tests for the
///       partitioning class

#include <cstdint>
#include <memory>
#include <fstream>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <stub/call.hpp>

#include <kodo/object/partitioning.hpp>

namespace kodo
{
// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    class dummy_partitioning
    {
    public:

        dummy_partitioning()
        {}

        dummy_partitioning(uint32_t max_symbols, uint32_t max_symbol_size,
                           uint32_t object_size)
        {
            m_constructor(max_symbols, max_symbol_size, object_size);

            m_blocks.set_return(1U);
            m_symbols.set_return(2U);
            m_symbol_size.set_return(3U);
            m_bytes_used.set_return(4U);
            m_byte_offset.set_return(5U);
            m_block_size.set_return(6U);
        }

        uint32_t blocks() const
        {
            return m_blocks();
        }

        uint32_t symbols(uint32_t index) const
        {
            return m_symbols(index);
        }

        uint32_t symbol_size(uint32_t index) const
        {
            return m_symbol_size(index);
        }

        uint32_t bytes_used(uint32_t index) const
        {
            return m_bytes_used(index);
        }

        uint32_t byte_offset(uint32_t index) const
        {
            return m_byte_offset(index);
        }

        uint32_t block_size(uint32_t index) const
        {
            return m_block_size(index);
        }

    public:

        stub::call<void(uint32_t,uint32_t,uint32_t)> m_constructor;
        stub::call<uint32_t()> m_blocks;
        stub::call<uint32_t(uint32_t)> m_symbols;
        stub::call<uint32_t(uint32_t)> m_symbol_size;
        stub::call<uint32_t(uint32_t)> m_bytes_used;
        stub::call<uint32_t(uint32_t)> m_byte_offset;
        stub::call<uint32_t(uint32_t)> m_block_size;
    };

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

            uint32_t object_size() const
            {
                return m_object_size();
            }

            stub::call<uint32_t()> m_object_size;
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
        }

    };

    // Helper stack
    class dummy_stack : public
        object::partitioning<dummy_partitioning, dummy_layer>
    { };
}
}

/// Test that the partitioning layer works as expected
TEST(ObjectTestPartitioning, api)
{
    uint32_t max_symbols = 10U;
    uint32_t max_symbol_size = 100U;

    kodo::dummy_stack::factory_base factory(max_symbols, max_symbol_size);

    uint32_t object_size = 12345;
    factory.m_object_size.set_return(object_size);

    EXPECT_EQ(factory.max_symbols(), max_symbols);
    EXPECT_EQ(factory.max_symbol_size(), max_symbol_size);

    kodo::dummy_stack stack;
    stack.initialize(factory);

    const auto& partitioning = stack.partitioning_scheme();
    EXPECT_TRUE((bool) partitioning.m_constructor.expect_calls()
                    .with(max_symbols, max_symbol_size, object_size));

    EXPECT_EQ(stack.blocks(), 1U);
    EXPECT_EQ(stack.symbols(1U), 2U);
    EXPECT_EQ(stack.symbol_size(1U), 3U);
    EXPECT_EQ(stack.bytes_used(1U), 4U);
    EXPECT_EQ(stack.byte_offset(1U), 5U);
    EXPECT_EQ(stack.block_size(1U), 6U);
}
