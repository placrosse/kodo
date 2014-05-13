// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_fulcrum_rank_inner_decoder.cpp Unit tests for the
/// fulcrum rank inner decoder

#include <gtest/gtest.h>

#include <kodo/rlnc/fulcrum_rank_inner_decoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        class nested_stack
        {
        public:
            uint32_t rank() const
            {
                return 10;
            }
        };

        class dummy_layer
        {
        public:

            dummy_layer()
            : m_expansion(0)
            {

            }

            const nested_stack* nested() const
            {
                return &m_nested_stack;
            }

            void set_expansion(uint32_t expansion)
            {
                m_expansion = expansion;
            }

            uint32_t expansion() const
            {
                return m_expansion;
            }

            uint32_t m_expansion;

            nested_stack m_nested_stack;
        };

        class dummy_stack
            : public fulcrum_rank_inner_decoder<dummy_layer>
        { };
    }
}

TEST(TestFulcrumRankInnerDecoder, api)
{
    typedef kodo::dummy_stack test_stack;

    test_stack stack;
    EXPECT_EQ(10U, stack.rank());
    stack.set_expansion(5);
    EXPECT_EQ(5U, stack.rank());
    stack.set_expansion(10);
    EXPECT_EQ(0U, stack.rank());
    stack.set_expansion(11);
    EXPECT_EQ(0U, stack.rank());
}
