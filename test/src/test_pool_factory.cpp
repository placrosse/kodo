// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_basic_factory.cpp Unit tests for the basic_factory class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/pool_factory.hpp>

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

            template<class Factory>
            void deinitialize(Factory& the_factory)
            {
                the_factory.member();
                m_deinitialize();
            }


        public:

            stub::call<void()> m_construct;
            stub::call<void()> m_initialize;
            stub::call<void()> m_deinitialize;

        };
    }
}

TEST(TestPoolFactory, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 15;

    kodo::pool_factory<kodo::dummy_stack> factory(symbols, symbol_size);
    EXPECT_EQ(factory.m_max_symbols, symbols);
    EXPECT_EQ(factory.m_max_symbol_size, symbol_size);

    // Check that we can call the members defined in the factory
    factory.member();
    EXPECT_EQ(factory.m_member.calls(), 1U);

    auto stack_one = factory.build();
    EXPECT_EQ(stack_one->m_construct.calls(), 1U);
    EXPECT_EQ(stack_one->m_initialize.calls(), 1U);
    EXPECT_EQ(stack_one->m_deinitialize.calls(), 0U);

    // We invoke the member in the construct and initialize
    EXPECT_EQ(factory.m_member.calls(), 3U);

    stack_one.reset();

    // We invoke the member in the deinitialize function
    EXPECT_EQ(factory.m_member.calls(), 4U);

    // Check that we get a recycled
    auto stack_two = factory.build();
    EXPECT_EQ(stack_two->m_construct.calls(), 1U);
    EXPECT_EQ(stack_two->m_initialize.calls(), 2U);
    EXPECT_EQ(stack_two->m_deinitialize.calls(), 1U);

    // We invoke the member in the initialize function
    EXPECT_EQ(factory.m_member.calls(), 5U);

    stack_two.reset();

    // We invoke the member in the deinitialize function
    EXPECT_EQ(factory.m_member.calls(), 6U);

    auto stack_three = factory.build();
    EXPECT_EQ(stack_three->m_construct.calls(), 1U);
    EXPECT_EQ(stack_three->m_initialize.calls(), 3U);
    EXPECT_EQ(stack_three->m_deinitialize.calls(), 2U);

    // We invoke the member in the initialize function
    EXPECT_EQ(factory.m_member.calls(), 7U);

    stack_three.reset();

    // We invoke the member in the deinitialize function
    EXPECT_EQ(factory.m_member.calls(), 8U);
}
