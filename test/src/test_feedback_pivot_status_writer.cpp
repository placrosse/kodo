// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_feedback_pivot_status_writer.cpp Unit tests for the
///       feedback_pivot_status_writer layer.

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/feedback_pivot_status_writer.hpp>

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

            void write_pivot_status(uint8_t* feedback) const
            {
                m_write_pivot_status(feedback);
            }

            uint32_t pivot_status_size() const
            {
                return m_pivot_status_size();
            }

            uint32_t write_feedback(uint8_t* buffer) const
            {
                return m_write_feedback(buffer);
            }

            stub::call<void(uint8_t*)> m_write_pivot_status;
            stub::call<uint32_t()> m_pivot_status_size;
            stub::call<uint32_t(uint8_t*)> m_write_feedback;

        };

        // Instantiate a stack containing the pivot_status_bitset
        class dummy_stack :
            public feedback_pivot_status_writer<dummy_layer>
        { };
    }
}

TEST(TestFeedbackPivotStatusWriter, api)
{
    kodo::dummy_stack stack;
    stack.m_pivot_status_size.set_return(10U);
    stack.m_write_feedback.set_return(2U);

    std::vector<uint8_t> data(20);

    uint32_t written = stack.write_feedback(data.data());
    EXPECT_EQ(written, 12U);

    EXPECT_TRUE((bool) stack.m_write_pivot_status.expect_calls()
                    .with(data.data()));

    EXPECT_TRUE((bool) stack.m_write_feedback.expect_calls()
                    .with(data.data() + 10U));
}
