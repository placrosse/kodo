// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_sliding_window_generator.cpp Unit tests for the
///       kodo::sliding_window_generator

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/rlnc/sliding_window_generator.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // sliding_window_generator layer.
        struct dummy_layer
        {

            typedef fifi::binary field_type;
            typedef typename field_type::value_type value_type;

            uint32_t coefficient_vector_size() const
            {
                return m_coefficient_vector_size;
            }

            uint32_t rank() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_is_symbol_pivot[i];
                }
                return rank;
            }

            uint32_t remote_rank() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_remote_is_symbol_pivot[i];
                }

                return rank;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            bool is_symbol_pivot(uint32_t index) const
            {
                return m_is_symbol_pivot[index];
            }

            bool remote_is_symbol_pivot(uint32_t index) const
            {
                return m_remote_is_symbol_pivot[index];
            }

            uint32_t m_symbols;
            uint32_t m_coefficient_vector_size;
            std::vector<bool> m_is_symbol_pivot;
            std::vector<bool> m_remote_is_symbol_pivot;

        };

        // Instantiate a stack containing the sliding_window_generator
        class dummy_stack
            : public sliding_window_generator<
                     dummy_layer>
        { };

    }
}


/// Run the tests typical coefficients stack
TEST(TestSlidingWindowGenerator, api)
{
    kodo::dummy_stack stack;

    stack.m_symbols = 5;
    stack.m_coefficient_vector_size = 10;
    stack.m_is_symbol_pivot.resize(stack.m_symbols, false);
    stack.m_remote_is_symbol_pivot.resize(stack.m_symbols, false);

    std::vector<uint8_t> buffer(stack.m_coefficient_vector_size);

    EXPECT_FALSE(stack.can_generate());

    stack.m_is_symbol_pivot[0] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_FALSE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_FALSE(stack.can_generate(4));

    stack.m_is_symbol_pivot[1] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_FALSE(stack.can_generate(4));

    stack.m_is_symbol_pivot[2] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_TRUE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_FALSE(stack.can_generate(4));

    stack.m_is_symbol_pivot[3] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_TRUE(stack.can_generate(2));
    EXPECT_TRUE(stack.can_generate(3));
    EXPECT_FALSE(stack.can_generate(4));

    stack.m_is_symbol_pivot[4] = true;
    EXPECT_TRUE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_TRUE(stack.can_generate(2));
    EXPECT_TRUE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    // Now start specifying some remote pivot positions

    stack.m_remote_is_symbol_pivot[0] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_TRUE(stack.can_generate(2));
    EXPECT_TRUE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    stack.m_remote_is_symbol_pivot[1] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_FALSE(stack.can_generate(1));
    EXPECT_TRUE(stack.can_generate(2));
    EXPECT_TRUE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    stack.m_remote_is_symbol_pivot[2] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_FALSE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_TRUE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    stack.m_remote_is_symbol_pivot[3] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_FALSE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    // In this case we cannot generate anything useful since the
    // receiver already has all the information we want to send.
    // This situation should be avoided in real applications.

    stack.m_remote_is_symbol_pivot[4] = true;
    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_FALSE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_FALSE(stack.can_generate(4));

}
