// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_final_feedback_reader.cpp Unit tests for the
///       final_feedback_reader layer.

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/final_feedback_reader.hpp>

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
        class dummy_stack : public final_feedback_reader<dummy_layer>
        { };

    }
}

TEST(TestFinalFeedbackReader, api)
{
    kodo::dummy_stack stack;

    uint8_t ptr;
    stack.read_feedback(&ptr);
}
