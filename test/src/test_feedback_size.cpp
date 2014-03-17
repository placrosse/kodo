// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_feedback_size.cpp Unit tests for the feedback_size() function

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/feedback_size.hpp>
#include <kodo/rlnc/full_vector_codes.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_one
        {
            uint32_t feedback_size() const
            {
                return 10;
            }
        };

        struct dummy_two
        { };

    }
}

TEST(TestFeedbackSize, invoke)
{

    {
        // Check with a standard object
        kodo::dummy_one d;
        EXPECT_EQ(kodo::feedback_size(d), 10U);
    }

    {
        // Check that the code compiles even with an object that
        // does not have the feedback_size function
        kodo::dummy_two d;

        if(kodo::has_feedback_size<kodo::dummy_two>::value)
        {
            EXPECT_EQ(kodo::feedback_size(d), 0U);
        }
    }

    {
        // Check that the code compiles with a normal stack
        // typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

        // encoder_type::factory factory(10,10);
        // auto encoder = factory.build();

        // EXPECT_EQ(kodo::feedback_size(encoder), 0U);
    }

}



