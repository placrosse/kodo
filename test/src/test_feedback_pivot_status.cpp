// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_feedback_pivot_status.cpp Unit tests for the
///       feedback_pivot_status layer.

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/feedback_pivot_status.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        struct dummy_layer
        {
        public:

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                uint32_t max_feedback_size() const
                {
                    return m_max_feedback_size();
                }

                uint32_t max_pivot_status_size() const
                {
                    return m_max_pivot_status_size();
                }

                stub::call<uint32_t()> m_max_feedback_size;
                stub::call<uint32_t()> m_max_pivot_status_size;
            };

        public:

            uint32_t pivot_status_size() const
            {
                return m_pivot_status_size();
            }

            uint32_t feedback_size() const
            {
                return m_feedback_size();
            }

            stub::call<uint32_t()> m_pivot_status_size;
            stub::call<uint32_t()> m_feedback_size;

        };

        // Instantiate a stack containing the pivot_status_bitset
        class dummy_stack : public feedback_pivot_status<dummy_layer>
        { };
    }
}

TEST(TestFeedbackPivotStatus, api)
{
    kodo::dummy_stack::factory_base factory(10, 10);
    factory.m_max_feedback_size.set_return(10U);
    factory.m_max_pivot_status_size.set_return(5U);

    EXPECT_EQ(factory.max_feedback_size(), 15U);

    kodo::dummy_stack stack;
    stack.m_pivot_status_size.set_return(12U);
    stack.m_feedback_size.set_return(13U);

    EXPECT_EQ(stack.feedback_size(), 25U);
}
