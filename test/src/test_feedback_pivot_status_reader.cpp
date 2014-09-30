// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_feedback_pivot_status_reader.cpp Unit tests for the
///       feedback_pivot_status_reader layer.

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/feedback_pivot_status_reader.hpp>

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

            void read_pivot_status(const uint8_t* feedback)
            {
                m_read_pivot_status(feedback);
            }

            uint32_t pivot_status_size() const
            {
                return m_pivot_status_size();
            }

            void read_feedback(const uint8_t* buffer)
            {
                m_read_feedback(buffer);
            }

            stub::call<void(const uint8_t*)> m_read_pivot_status;
            stub::call<uint32_t()> m_pivot_status_size;
            stub::call<void(const uint8_t*)> m_read_feedback;
        };

        // Instantiate a stack containing the feedback_pivot_status_reader
        class dummy_stack : public feedback_pivot_status_reader<dummy_layer>
        { };
    }
}

TEST(TestFeedbackPivotStatusReader, api)
{
    kodo::dummy_stack stack;

    stack.m_pivot_status_size.set_return(10U);

    std::vector<uint8_t> data(20);
    stack.read_feedback(data.data());

    EXPECT_TRUE((bool) stack.m_read_pivot_status.expect_calls()
                    .with(data.data()));

    EXPECT_TRUE((bool) stack.m_read_feedback.expect_calls()
                    .with(data.data() + 10));
}
