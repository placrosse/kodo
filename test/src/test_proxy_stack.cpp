// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <kodo/proxy_stack.hpp>
#include <kodo/proxy_layer.hpp>
#include <kodo/basic_factory.hpp>
#include <kodo/final_layer.hpp>

#include "kodo_unit_test/helper_test_nested_stack.hpp"

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // We have tried to make this unit test as stand-alone and
        // compact as possible.
        //
        // The dummy_proxy_super layer "fakes" both the nested stack
        // and the nested stack layer. So that is why the nested()
        // function returns this.
        //
        template<class NestedStack, class SuperCoder>
        class dummy_proxy_super
        {
        public:

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_main_factory(nullptr),
                      m_main_stack(nullptr)

                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                factory_base& nested()
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

        /// This stack is the proxy stack i.e. the stack which is
        /// embedded with the main stack.
        // template<class MainStack>
        // class dummy_proxy_stack : public
        //     proxy_layer<MainStack,final_layer>
        // {
        // public:
        //     using factory = basic_factory<dummy_proxy_stack>;
        // };

        // /// The stack represents the main stack used in unit test
        // class dummy_stack : public
        //     proxy_stack<proxy_args<>, dummy_proxy_stack,
        //     main_stack_types<helper_nested_layer> >
        // {
        // public:
        //     using factory = basic_factory<dummy_stack>;
        // };
    }
}

TEST(TestProxyStack, api)
{
    /// @todo merge this an basic_proxy_stack. The basic proxy stack
    // is just a alias and this unit-test currently does not use the
    // up-to-date proxystack


    // uint32_t symbols = 16; uint32_t
    // symbol_size = 1400;

    // kodo::dummy_stack::factory factory(symbols, symbol_size);
    // EXPECT_TRUE(factory.m_main_factory != nullptr);
    // EXPECT_TRUE(factory.m_main_stack == nullptr);

    // kodo::dummy_stack stack;
    // stack.initialize(factory);

    // EXPECT_TRUE(factory.m_main_factory != nullptr);
    // EXPECT_TRUE(factory.m_main_stack != nullptr);

    // EXPECT_TRUE(stack.m_main_stack != nullptr);
}
