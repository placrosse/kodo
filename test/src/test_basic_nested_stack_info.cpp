// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>
#include <kodo/basic_nested_stack_info.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // Testing dummy layer
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

                uint32_t max_symbols() const
                {
                    return 5;
                }

                uint32_t max_symbol_size() const
                {
                    return 3;
                }
            };

        public:

            uint32_t symbols() const
            {
                return 2;
            }

            uint32_t symbol_size() const
            {
                return 7;
            }
        };

        // Helper stack
        class dummy_stack : public
            basic_nested_stack_info<dummy_layer>
        { };

    }
}


TEST(TestBasicNestedStackInfo, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 10;

    kodo::dummy_stack::factory_base factory(symbols, symbol_size);

    EXPECT_EQ(factory.nested_max_symbols(), 5U);
    EXPECT_EQ(factory.nested_max_symbol_size(), 3U);

    kodo::dummy_stack stack;

    EXPECT_EQ(stack.nested_symbols(), 2U);
    EXPECT_EQ(stack.nested_symbol_size(), 7U);
}
