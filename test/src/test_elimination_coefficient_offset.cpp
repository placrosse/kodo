// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/elimination_coefficient_offset.hpp>
#include <kodo/storage_block_size.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        /// Helper class to test coefficient value access layer
        class test_layer
        {
        public:

            class factory_base
            {
            public:
                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_symbols(0),
                      m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t max_symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t symbols() const
                {
                    return m_symbols;
                }

            public:

                uint32_t m_symbols;
                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;
            };

        public:

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }

        };

        /// Helper stack for testing the coefficient access layer
        class test_stack :
            public elimination_coefficient_offset<test_layer>
        {
        public:
            using factory = basic_factory<test_stack>;
        };
    }
}

TEST(TestEliminationCoefficientOffset, api)
{

    uint32_t symbols = 10;
    uint32_t max_symbols = 18;
    uint32_t max_symbol_size = 1300;

    kodo::test_stack::factory factory(max_symbols, max_symbol_size);
    factory.m_symbols = symbols;

    EXPECT_EQ(factory.elimination_offset(), 0U);
    EXPECT_EQ(factory.symbols(), symbols);

    uint32_t elimination_offset = 6;
    factory.set_elimination_offset(elimination_offset);

    EXPECT_EQ(factory.elimination_offset(), elimination_offset);

    kodo::test_stack stack;
    stack.initialize(factory);

    EXPECT_EQ(stack.elimination_offset(), elimination_offset);

    factory.set_elimination_offset(0);
    stack.initialize(factory);

    EXPECT_EQ(factory.elimination_offset(), 0U);
    EXPECT_EQ(stack.elimination_offset(), 0U);
}
