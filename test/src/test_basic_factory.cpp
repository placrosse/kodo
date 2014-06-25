// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_basic_factory.cpp Unit tests for the basic_factory class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/basic_factory.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        /// Helper stack
        class dummy_stack
        {
        public:

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                void member()
                {
                    m_member();
                }

            public:

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

                stub::call<void()> m_member;
            };

        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                the_factory.member();
                m_construct();
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                the_factory.member();
                m_initialize();
            }

        public:

            stub::call<void()> m_initialize;
            stub::call<void()> m_construct;
        };
    }
}

TEST(TestBasicFactory, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 15;

    kodo::basic_factory<kodo::dummy_stack> factory(symbols, symbol_size);
    EXPECT_EQ(factory.m_max_symbols, symbols);
    EXPECT_EQ(factory.m_max_symbol_size, symbol_size);

    // Check that we can call the members defined in the factory
    factory.member();
    EXPECT_EQ(factory.m_member.calls(), 1U);

    auto stack_one = factory.build();
    EXPECT_EQ(stack_one->m_construct.calls(), 1U);
    EXPECT_EQ(stack_one->m_initialize.calls(), 1U);

    // We invoke the member in the construct and initialize stacks
    EXPECT_EQ(factory.m_member.calls(), 3U);

    // Check that we get a new stack
    auto stack_two = factory.build();
    EXPECT_EQ(stack_two->m_construct.calls(), 1U);
    EXPECT_EQ(stack_two->m_initialize.calls(), 1U);

    // We invoke the member in the construct and initialize stacks
    EXPECT_EQ(factory.m_member.calls(), 5U);
}
