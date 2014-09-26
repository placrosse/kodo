// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_remote_pivot_aware_systematic_phase.cpp Unit tests for the
///       kodo::remote_pivot_aware_systematic_phase layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/remote_pivot_aware_systematic_phase.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // remote_pivot_aware_systematic_phase layer.
        struct dummy_layer
        {
        public:

            bool in_systematic_phase() const
            {
                return m_in_systematic_phase;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            bool remote_is_symbol_pivot(uint32_t index) const
            {
                return m_remote_is_symbol_pivot[index];
            }

            bool is_symbol_uncoded(uint32_t index) const
            {
                return m_is_symbol_uncoded[index];
            }

            uint32_t next_systematic_symbol() const
            {
                return m_next_systematic_symbol;
            }

            bool m_in_systematic_phase;
            uint32_t m_symbols;

            std::vector<bool> m_remote_is_symbol_pivot;
            std::vector<bool> m_is_symbol_uncoded;

            uint32_t m_next_systematic_symbol;
        };

        // Test stack
        class dummy_stack
            : public remote_pivot_aware_systematic_phase<
                     dummy_layer>
        { };
    }
}


/// Run the tests typical coefficients stack
TEST(TestRemotePivotAwareSystematicPhase, api)
{
    kodo::dummy_stack stack;

    uint32_t symbols = 5;

    stack.m_in_systematic_phase = false;
    stack.m_symbols = symbols;

    stack.m_remote_is_symbol_pivot.resize(symbols, false);
    stack.m_is_symbol_uncoded.resize(symbols, false);

    stack.m_next_systematic_symbol = 9;

    EXPECT_EQ(stack.m_symbols, symbols);

    // The layer we are testing will check the differences between the
    // remote and local pivots. If there is only a single symbol
    // difference it will be returned as systematic

    EXPECT_FALSE(stack.in_systematic_phase());
    EXPECT_FALSE(stack.single_symbol_available());

    stack.m_in_systematic_phase = true;

    EXPECT_TRUE(stack.in_systematic_phase());
    EXPECT_EQ(stack.next_systematic_symbol(), 9U);
    EXPECT_FALSE(stack.single_symbol_available());

    stack.m_is_symbol_uncoded[1] = true;

    EXPECT_TRUE(stack.in_systematic_phase());
    EXPECT_EQ(stack.next_systematic_symbol(), 1U);
    EXPECT_TRUE(stack.single_symbol_available());

    stack.m_remote_is_symbol_pivot[0] = true;

    // There is still a single symbol difference between what the
    // source has and what the receiver has

    EXPECT_TRUE(stack.in_systematic_phase());
    EXPECT_EQ(stack.next_systematic_symbol(), 1U);
    EXPECT_TRUE(stack.single_symbol_available());

    stack.m_remote_is_symbol_pivot[1] = true;

    // There is no difference anymore in symbols available (the
    // receiver actually knows more than the sender) however we are
    // still in systematic because the lower layer signals it

    EXPECT_TRUE(stack.in_systematic_phase());
    EXPECT_EQ(stack.next_systematic_symbol(), 9U);
    EXPECT_FALSE(stack.single_symbol_available());

    stack.m_in_systematic_phase = false;

    EXPECT_FALSE(stack.in_systematic_phase());
    EXPECT_FALSE(stack.single_symbol_available());



}
