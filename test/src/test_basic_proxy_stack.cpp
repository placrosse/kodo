// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <gtest/gtest.h>
#include <fifi/binary8.hpp>

#include <kodo/basic_proxy_stack.hpp>
#include <kodo/proxy_layer.hpp>
#include <kodo/final_layer.hpp>

#include "kodo_unit_test/helper_test_nested_stack.hpp"

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        /// The proxy_layer class which provides the API required by
        /// the proxy_stack code requires the "main stack" to provide
        /// two typedefs (which are the ones added here)
        template<class SuperCoder>
        class main_stack_types : public SuperCoder
        {
        public:

            typedef fifi::binary8 field_type;
            typedef field_type::value_type value_type;

        public:

            // We implement this function here to check whether
            // function calls not implemented in the nested stack are
            // correctly forwarded to the main stack
            uint32_t block_size() const
            {
                return 100;
            }

        };

        /// This stack is the proxy stack i.e. the stack which is
        /// embedded with the main stack.
        template<class MainStack>
        class dummy_proxy_stack : public
              proxy_layer<MainStack, final_layer>
        { };

        /// The stack represents the main stack used in unit test
        class dummy_stack : public
            basic_proxy_stack<proxy_args<>, dummy_proxy_stack,
            main_stack_types<helper_nested_layer> >
        { };
    }
}

TEST(TestBasicProxyStack, api)
{
    uint32_t symbols = 16;
    uint32_t symbol_size = 1400;

    kodo::dummy_stack::factory_base factory(symbols, symbol_size);
    EXPECT_EQ(factory.m_max_symbols, symbols);
    EXPECT_EQ(factory.m_max_symbol_size, symbol_size);

    auto& nested_factory = factory.nested();
    EXPECT_EQ(nested_factory.max_symbols(), symbols);
    EXPECT_EQ(nested_factory.max_symbol_size(), symbol_size);

    kodo::dummy_stack stack;

    stack.construct(factory);
    stack.initialize(factory);

    auto nested = stack.nested();
    assert(nested);

    EXPECT_EQ(nested->main_stack(), &stack);

    EXPECT_EQ(nested->symbols(), symbols);
    EXPECT_EQ(nested->symbol_size(), symbol_size);
    EXPECT_EQ(nested->block_size(), 100U);

    // Try to adjust the symbols and symbol size. If we now initialize
    // the stack again we should see the change refelected in the
    // nested stack
    factory.m_symbols = 10;
    factory.m_symbol_size = 100;

    stack.initialize(factory);

    nested = stack.nested();
    assert(nested);

    EXPECT_EQ(nested->main_stack(), &stack);
    EXPECT_EQ(nested->symbols(), factory.m_symbols);
    EXPECT_EQ(nested->symbol_size(), factory.m_symbol_size);
    EXPECT_EQ(nested->block_size(), 100U);

    stack.initialize(factory);

    nested = stack.nested();
    assert(nested);

    EXPECT_EQ(nested->main_stack(), &stack);
    EXPECT_EQ(nested->symbols(), factory.m_symbols);
    EXPECT_EQ(nested->symbol_size(), factory.m_symbol_size);
    EXPECT_EQ(nested->block_size(), 100U);
}
