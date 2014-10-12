// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <stub/call.hpp>

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
                return m_max_payload_size();
            }

            stub::call<uint32_t ()> m_max_payload_size;
        };

        // This will represent the nested stack returned by the actual
        // stack
        struct nested_stack
        {
            uint32_t payload_size() const
            {
                return m_payload_size();
            }

            void decode(uint8_t* payload)
            {
                m_decode(payload);
            }

            stub::call<uint32_t ()> m_payload_size;
            stub::call<void (uint8_t*)> m_decode;
        };

        // The layer providing the API required by the
        // nested_payload_decoder
        class dummy_layer
        {
        public:

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                const nested_factory& nested() const
                {
                    return m_nested;
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

                nested_factory m_nested;
            };

        public:

            nested_stack* nested()
            {
                return &m_nested;
            }

            const nested_stack* nested() const
            {
                return &m_nested;
            }

            nested_stack m_nested;
        };

        // Helper stack
        class dummy_stack : public
            nested_payload_decoder<dummy_layer>
        { };

    }
}

TEST(TestNestedPayloadDecoder, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 10;

    kodo::dummy_stack::factory_base factory(symbols, symbol_size);
    EXPECT_EQ(factory.m_max_symbols, symbols);
    EXPECT_EQ(factory.m_max_symbol_size, symbol_size);

    factory.m_nested.m_max_payload_size.set_return(100);
    EXPECT_EQ(factory.max_payload_size(), 100);

    kodo::dummy_stack stack;
    stack.m_nested.m_payload_size.set_return(1000);
    EXPECT_EQ(stack.payload_size(), 1000);

    stack.decode((uint8_t*)0xa);
    EXPECT_TRUE((bool) stack.m_nested.m_decode.expect_calls()
                    .with((uint8_t*)0xa));
}
