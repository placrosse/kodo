// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_proxy_seen_encoder_rank.cpp Unit tests for the proxy layer class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/proxy_seen_encoder_rank.hpp>

namespace kodo
{

    // Object to represent the main stack, which the proxy will
    // access to call the seen_encoder_rank() function.
    class dummy_main_stack
    {
    public:

        typedef uint32_t rank_type;

    public:

        rank_type seen_encoder_rank() const
        {
            return m_seen_encoder_rank;
        }

    public:

        uint32_t m_seen_encoder_rank;
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

            factory() : m_main_stack_pointer(&m_main_stack)
            {
            }

            const main_stack* proxy_stack() const
            {
                return m_main_stack_pointer;
            }

        public:

            main_stack m_main_stack;
            main_stack* m_main_stack_pointer;

        };

        template<class Factory>
        void initialize(Factory& /*the_factory*/)
        { }

    };


    class test_stack : public proxy_seen_encoder_rank<dummy_layer>
    { };

}

TEST(TestProxySeenEncoderRank, test)
{

    kodo::test_stack::factory factory;
    kodo::test_stack stack;
    stack.initialize(factory);

    factory.m_main_stack.m_seen_encoder_rank = 0U;
    EXPECT_EQ(stack.seen_encoder_rank(), 0U);

    factory.m_main_stack.m_seen_encoder_rank = 50U;
    EXPECT_EQ(stack.seen_encoder_rank(), 50U);

    factory.m_main_stack.m_seen_encoder_rank = 5U;
    EXPECT_EQ(stack.seen_encoder_rank(), 5U);

    factory.m_main_stack.m_seen_encoder_rank = 0U;
    EXPECT_EQ(stack.seen_encoder_rank(), 0U);

}


