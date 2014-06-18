// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_final_feedback_size.cpp Unit tests for the final_feedback_size
///       layer.

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/final_feedback_size.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_layer
        {

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

            };

        };

        // Instantiate a stack containing the pivot_status_bitset
        class dummy_stack : public final_feedback_size<dummy_layer>
        {
        public:
            using factory = basic_factory<dummy_stack>;
        };


    }
}

TEST(TestFinalFeedbackSize, api)
{
    kodo::dummy_stack stack;
    kodo::dummy_stack::factory factory(10, 10);

    EXPECT_EQ(stack.feedback_size(), 0U);
    EXPECT_EQ(factory.max_feedback_size(), 0U);
}
