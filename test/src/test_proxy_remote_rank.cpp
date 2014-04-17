// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_proxy_remote_rank.cpp Unit tests for the proxy layer class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/proxy_remote_rank.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Object to represent the main stack, which the proxy will
        // access to call the remote_rank() function.
        class dummy_main_stack
        {
        public:

            typedef uint32_t rank_type;

        public:

            rank_type remote_rank() const
            {
                return m_remote_rank;
            }

        public:

            uint32_t m_remote_rank;
        };


        // Dummy layer which exposes the main stack pointers
        class dummy_layer
        {
        public:

            typedef dummy_main_stack main_stack;

        public:

            class factory
            {
            public:

                // factory() : m_main_stack_pointer(&m_main_stack)
                // { }

                const main_stack* proxy_stack() const
                {
                    return m_main_stack_pointer;
                }

            public:

                main_stack* m_main_stack_pointer;

            };

            template<class Factory>
            void initialize(Factory& /*the_factory*/)
            { }

        };


        class test_stack : public proxy_remote_rank<dummy_layer>
        {
        public:

            typedef proxy_remote_rank<dummy_layer> Super;
            typedef Super::main_stack main_stack;

            const main_stack* stack() const
            {
                return Super::m_proxy;
            }

        };
    }
}

TEST(TestProxyRemoteRank, api)
{
    kodo::test_stack::factory factory;

    // Initialize the factory with a specific main_stack
    kodo::dummy_main_stack main_stack_one;
    factory.m_main_stack_pointer = &main_stack_one;

    // Initialize the "test stack" i.e. the stack implementing the
    // proxy_remote_rank layer
    kodo::test_stack stack;
    stack.initialize(factory);

    EXPECT_EQ(stack.stack(), &main_stack_one);

    main_stack_one.m_remote_rank = 0U;
    EXPECT_EQ(stack.remote_rank(), 0U);

    main_stack_one.m_remote_rank = 50U;
    EXPECT_EQ(stack.remote_rank(), 50U);

    main_stack_one.m_remote_rank = 5U;
    EXPECT_EQ(stack.remote_rank(), 5U);

    main_stack_one.m_remote_rank = 0U;
    EXPECT_EQ(stack.remote_rank(), 0U);



}


