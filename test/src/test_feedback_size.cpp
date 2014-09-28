// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_feedback_size.cpp Unit tests for the feedback_size() function

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/feedback_size.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>

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
        using decoder_type = kodo::sliding_window_decoder<fifi::binary8>;
        kodo::basic_factory<decoder_type> factory(10,10);

        auto decoder = factory.build();


        EXPECT_TRUE(kodo::feedback_size(decoder) > 0U);
    }
}
