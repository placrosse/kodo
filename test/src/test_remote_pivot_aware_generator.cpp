// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_remote_pivot_aware_generator.cpp Unit tests for the
///       kodo::remote_pivot_aware_generator layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/remote_pivot_aware_generator.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // remote_pivot_aware_generator layer.
        struct dummy_layer
        {
            typedef uint32_t rank_type;

            rank_type remote_decoder_rank() const
            {
                rank_type rank = 0;
                for(uint32_t i = 0; i < m_remote_is_symbol_pivot.size(); ++i)
                {
                    rank += m_remote_is_symbol_pivot[i];
                }
                return rank;
            }

            bool can_generate() const
            {
                return m_can_generate;
            }

            bool remote_is_symbol_pivot(uint32_t index) const
            {
                return m_remote_is_symbol_pivot[index];
            }

            bool can_generate(uint32_t index) const
            {
                return m_is_symbol_pivot[index];
            }

            bool m_can_generate;

            std::vector<bool> m_remote_is_symbol_pivot;
            std::vector<bool> m_is_symbol_pivot;

        };

        // Instantiate a stack containing the remote_pivot_aware_generator
        class dummy_stack
            : public remote_pivot_aware_generator<
                     dummy_layer>
          { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestRemotePivotAwareGenerator, api)
{

    kodo::dummy_stack stack;

    uint32_t symbols = 10;

    stack.m_remote_is_symbol_pivot.resize(symbols, false);
    stack.m_is_symbol_pivot.resize(symbols, false);

    stack.m_can_generate = false;
    EXPECT_EQ(stack.can_generate(), false);

    stack.m_can_generate = true;
    EXPECT_EQ(stack.can_generate(), true);

    stack.m_is_symbol_pivot[3] = true;

    EXPECT_EQ(stack.can_generate(3), true);
    stack.m_remote_is_symbol_pivot[3] = true;

    EXPECT_EQ(stack.can_generate(3), false);

    EXPECT_EQ(stack.remote_decoder_rank(), 1U);
    EXPECT_EQ(stack.can_generate(), false);
    stack.m_can_generate = false;

    EXPECT_EQ(stack.can_generate(), false);
}
