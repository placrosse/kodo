// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <kodo/nested_payload_decoder.hpp>
#include "kodo_unit_test/helper_test_nested_stack.hpp"

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // This will represent the nested factory returned by the
        // stack factory
        struct nested_factory
        {
            uint32_t max_payload_size() const
            {
                return m_max_payload_size;
            }

            uint32_t m_max_payload_size;
        };

        // This will represent the nested stack returned by the actual
        // stack
        struct nested_stack
        {
            uint32_t payload_size() const
            {
                return m_payload_size;
            }

            void decode(uint8_t* payload)
            {
                m_payload = payload;
            }

            uint32_t m_payload_size;
            uint8_t* m_payload;
        };

        // The layer providing the API required by the
        // nested_payload_decoder
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

                uint32_t max_payload_size() const
                {
                    return m_max_payload_size;
                }

                nested_factory& nested() const
                {
                    return m_nested;
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

                uint32_t m_max_payload_size;

                nested_factory m_nested;
            };

        public:

            uint32_t payload_size() const
            {
                return m_payload_size;
            }

            nested* nested
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

    kodo::dummy_stack stack;
    stack.initialize(factory);

    EXPECT_TRUE(stack.nested() != 0);
    EXPECT_EQ(stack.nested()->m_symbols, 2U);
    EXPECT_EQ(stack.nested()->m_symbol_size, 7U);
}
