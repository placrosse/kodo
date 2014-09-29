// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_pivot_aware_generator.cpp Unit tests for the
///       kodo::pivot_aware_generator

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/pivot_aware_generator.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // pivot_aware_generator layer.
        struct dummy_layer
        {

            uint32_t rank() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_is_symbol_pivot[i];
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

            uint32_t m_symbols;
            std::vector<bool> m_is_symbol_pivot;

        };

        // Instantiate a stack containing the pivot_aware_generator
        class dummy_stack
            : public pivot_aware_generator<
                     dummy_layer>
        { };

    }
}


/// Run the tests typical coefficients stack
TEST(TestPivotAwareGenerator, api)
{
    kodo::dummy_stack stack;

    stack.m_symbols = 5;
    stack.m_is_symbol_pivot.resize(stack.m_symbols, false);

    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_FALSE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_FALSE(stack.can_generate(4));

    stack.m_is_symbol_pivot[1] = true;

    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_FALSE(stack.can_generate(4));

    stack.m_is_symbol_pivot[4] = true;

    EXPECT_FALSE(stack.can_generate());

    EXPECT_FALSE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    stack.m_is_symbol_pivot[0] = true;

    EXPECT_FALSE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_FALSE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    stack.m_is_symbol_pivot[2] = true;

    EXPECT_FALSE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_TRUE(stack.can_generate(2));
    EXPECT_FALSE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

    stack.m_is_symbol_pivot[3] = true;

    EXPECT_TRUE(stack.can_generate());

    EXPECT_TRUE(stack.can_generate(0));
    EXPECT_TRUE(stack.can_generate(1));
    EXPECT_TRUE(stack.can_generate(2));
    EXPECT_TRUE(stack.can_generate(3));
    EXPECT_TRUE(stack.can_generate(4));

}
