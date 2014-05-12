// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <kodo/nested_stack.hpp>
#include "kodo_unit_test/helper_test_nested_stack.hpp"

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        class dummy_layer
        {
        public:

            class factory
            {
            public:

                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t nested_max_symbols() const
                {
                    return 5;
                }

                uint32_t nested_max_symbol_size() const
                {
                    return 9;
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;
            };

            uint32_t nested_symbol() const
            {
                return 2;
            }

            uint32_t nested_symbol_size() const
            {
                return 7;
            }
        };

        // Helper stack
        class dummy_stack : public
            nested_stack<helper_nested_stack,dummy_layer>
        { };

    }
}


TEST(TestNestedStack, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 10;

    kodo::dummy_stack::factory factory(symbols, symbol_size);
    EXPECT_EQ(factory.m_max_symbols, symbols);
    EXPECT_EQ(factory.m_max_symbol_size, symbol_size);
    EXPECT_EQ(factory.nested().m_max_symbols, 5U);
    EXPECT_EQ(factory.nested().m_max_symbol_size, 9U);

}
