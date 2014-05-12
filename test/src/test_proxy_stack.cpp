// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <gtest/gtest.h>
#include <fifi/binary8.hpp>

#include <kodo/basic_proxy_stack.hpp>
#include <kodo/proxy_layer.hpp>

#include "kodo_unit_test/helper_test_nested_stack.hpp"

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // The proxy_stack takes a number of templates here is an
        // overview of what types are expected where:
        //
        // ProxySuper<T,V>
        //     where T is the stack to be nested an V will
        //     be the SuperCoder of ProxySuper.
        //
        //     The ProxySuper should define a factory with a nested() function
        template<class NestedStack, class SuperCoder>
        class dummy_proxy_super
        {
        public:

            class factory
            {
            public:

                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_main_factory(nullptr),
                      m_main_stack(nullptr)

                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                factory& nested()
                {
                    return *this;
                }

                void* main_factory()
                {
                    return m_main_factory;
                }

                void* main_stack()
                {
                    return m_main_stack;
                }

                void set_main_factory(void* main_factory)
                {
                    m_main_factory = main_factory;
                }

                void set_main_stack(void* main_stack)
                {
                    m_main_stack = main_stack;
                }

                void* m_main_factory;
                void* m_main_stack;
            };

        public:

            dummy_proxy_super()
                : m_main_stack(nullptr)
            { }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_main_stack = the_factory.main_stack();
            }

            dummy_proxy_super* nested()
            {
                return this;
            }

            void* main_stack()
            {
                return m_main_stack;
            }

            void* m_main_stack;

        };

        template<class V>
        class dummy_nested
        {};

        class dummy_super
        {};

        /// The stack represents the main stack used in unit test
        class dummy_stack : public
            proxy_stack<dummy_proxy_super,
                        proxy_args<>,
                        dummy_nested,
                        dummy_super>
        { };

    }

}


TEST(TestProxyStack, api)
{
    uint32_t symbols = 16;
    uint32_t symbol_size = 1400;

    kodo::dummy_stack::factory factory(symbols, symbol_size);
    EXPECT_TRUE(factory.m_main_factory != nullptr);
    EXPECT_TRUE(factory.m_main_stack == nullptr);

    kodo::dummy_stack stack;
    stack.initialize(factory);

    EXPECT_TRUE(factory.m_main_factory != nullptr);
    EXPECT_TRUE(factory.m_main_stack != nullptr);

    EXPECT_TRUE(stack.m_main_stack != nullptr);


}
