// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <gtest/gtest.h>
#include <fifi/field_types.hpp>

#include <kodo/proxy_stack.hpp>
#include <kodo/proxy_layer.hpp>

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
        /// two typedefs (which are th ones added here)
        template<class SuperCoder>
        class main_stack_types : public SuperCoder
        {
        public:
            typedef fifi::binary8 field_type;
            typedef field_type::value_type value_type;
        };

        /// The nested_stack used by the proxy_stack layer calls the
        /// set_symbols() and set_symbol_size() functions when
        /// building the nested stack. This layer makes sure that
        /// these two functions exist even though they are unused. We
        /// maybe should perform some tests here.
        template<class SuperCoder>
        class proxy_symbols : public SuperCoder
        {
        public:

            class factory : public SuperCoder::factory
            {
            public:

                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : SuperCoder::factory(max_symbols, max_symbol_size)
                { }

                void set_symbols(uint32_t symbols)
                {
                    assert(symbols <=
                           SuperCoder::factory::max_symbols());

                    m_symbols = symbols;
                }

                void set_symbol_size(uint32_t symbol_size)
                {
                    assert(symbol_size <=
                           SuperCoder::factory::max_symbol_size());

                    m_symbol_size = symbol_size;
                }

            public:

                uint32_t m_symbols;
                uint32_t m_symbol_size;
            };

        };

        /// This stack is the proxy stack i.e. the stack which is
        /// embedded with the main stack.
        template<class MainStack>
        class dummy_proxy_stack : public
            proxy_symbols<
            proxy_layer<dummy_proxy_stack<MainStack>, MainStack> >
        { };

        /// The stack represents the main stack used in unit test
        class dummy_stack : public
            proxy_stack<dummy_proxy_stack,
            main_stack_types<helper_nested_layer> >
        { };

    }

}


TEST(TestProxyStack, api)
{
    uint32_t symbols = 16;
    uint32_t symbol_size = 1400;

    kodo::dummy_stack::factory factory(symbols, symbol_size);
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

    EXPECT_EQ(nested->proxy_stack(), &stack);

    EXPECT_EQ(nested->symbols(), symbols);
    EXPECT_EQ(nested->symbol_size(), symbol_size);

    // Try to adjust the symbols and symbol size. If we now initialize
    // the stack again we should see the change refelected in the
    // nested stack
    factory.m_symbols = 10;
    factory.m_symbol_size = 100;

    stack.initialize(factory);

    nested = stack.nested();
    assert(nested);

    EXPECT_EQ(nested->proxy_stack(), &stack);
    EXPECT_EQ(nested->symbols(), factory.m_symbols);
    EXPECT_EQ(nested->symbol_size(), factory.m_symbol_size);
}



