// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_check_partial_generator.cpp Unit tests for the
///       kodo::check_partial_generator

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/check_partial_generator.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // check_partial_generator layer.
        struct dummy_layer
        {
            bool can_generate() const
            {
                return m_can_generate;
            }

            void generate(uint8_t* /*coefficents*/)
            {
                m_generate = true;
            }

            void generate_partial(uint8_t* /*coefficents*/)
            {
                m_generate_partial = true;
            }

            void reset()
            {
                m_generate = false;
                m_generate_partial = false;
            }

            bool m_generate;
            bool m_generate_partial;

            bool m_can_generate;
        };

        // Instantiate a stack containing the check_partial_generator
        class dummy_stack
            : public check_partial_generator<
                     dummy_layer>
        { };

    }
}


/// Run the tests typical coefficients stack
TEST(TestCheckPartialGenerator, api)
{
    kodo::dummy_stack stack;
    stack.reset();

    stack.m_can_generate = false;

    std::vector<uint8_t> coefficients(1);

    stack.generate(&coefficients[0]);

    EXPECT_FALSE(stack.m_generate);
    EXPECT_TRUE(stack.m_generate_partial);

    stack.reset();

    stack.m_can_generate = true;

    stack.generate(&coefficients[0]);

    EXPECT_TRUE(stack.m_generate);
    EXPECT_FALSE(stack.m_generate_partial);

}
