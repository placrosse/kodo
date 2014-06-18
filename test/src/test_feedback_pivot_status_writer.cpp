// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_feedback_pivot_status_writer.cpp Unit tests for the
///       feedback_pivot_status_writer layer.

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/feedback_pivot_status_writer.hpp>
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
                    return m_max_feedback_size;
                }

                uint32_t max_pivot_status_size() const
                {
                    return m_max_pivot_status_size;
                }

                uint32_t m_max_feedback_size;
                uint32_t m_max_pivot_status_size;

            };

        public:

            void write_pivot_status(uint8_t* feedback) const
            {
                m_write_pivot_status = feedback;
            }

            uint32_t pivot_status_size() const
            {
                return m_pivot_status_size;
            }

            uint32_t write_feedback(uint8_t* buffer) const
            {
                m_write_feedback = buffer;
                return m_feedback_size;
            }

            uint32_t feedback_size() const
            {
                return m_feedback_size;
            }

            mutable uint8_t* m_write_pivot_status;
            uint32_t m_pivot_status_size;

            mutable uint8_t* m_write_feedback;
            uint32_t m_feedback_size;

        };

        // Instantiate a stack containing the pivot_status_bitset
        class dummy_stack :
            public feedback_pivot_status_writer<dummy_layer>
        {
        public:
            using factory = basic_factory<dummy_stack>;
        };
    }
}

TEST(TestFeedbackPivotStatusWriter, api)
{
    kodo::dummy_stack stack;
    kodo::dummy_stack::factory factory(10, 10);

    factory.m_max_feedback_size = 10;
    factory.m_max_pivot_status_size = 10;

    EXPECT_EQ(factory.max_feedback_size(), 20U);

    stack.m_pivot_status_size = 10;
    stack.m_feedback_size = 10;

    EXPECT_EQ(stack.feedback_size(), 20U);

    uint8_t ptr[20];
    EXPECT_EQ(stack.write_feedback(ptr), 20U);
    EXPECT_EQ(stack.m_write_pivot_status, &ptr[0]);
    EXPECT_EQ(stack.m_write_feedback, &ptr[10]);

}
