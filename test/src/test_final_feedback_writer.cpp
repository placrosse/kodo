// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_final_feedback_writer.cpp Unit tests for the
///       final_feedback_writer layer.

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/final_feedback_writer.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        struct dummy_layer
        { };

        // Instantiate a stack containing the pivot_status_bitset
        class dummy_stack : public final_feedback_writer<dummy_layer>
        { };
    }
}

TEST(TestFinalFeedbackWriter, api)
{
    kodo::dummy_stack stack;
    uint8_t ptr;

    EXPECT_EQ(stack.write_feedback(&ptr), 0U);
}
