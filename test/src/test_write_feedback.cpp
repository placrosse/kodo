// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_write_feedback.cpp Unit tests for the write_feedback() function

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/write_feedback.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_one
        {
            uint32_t write_feedback(uint8_t*) const
            {
                return 10;
            }
        };

        struct dummy_two
        { };

    }
}

TEST(TestWriteFeedback, invoke)
{

    uint8_t* ptr = 0;

    {
        // Check with a standard object
        kodo::dummy_one d;
        EXPECT_EQ(kodo::write_feedback(d, ptr), 10U);
    }

    {
        // Check that the code compiles even with an object that
        // does not have the write_feedback function
        kodo::dummy_two d;

        if(kodo::has_write_feedback<kodo::dummy_two>::value)
        {
            EXPECT_EQ(kodo::write_feedback(d, ptr), 0U);
        }
    }

    {
        // Check that the code compiles with a normal stack
        // typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

        // encoder_type::factory factory(10,10);
        // auto encoder = factory.build();

        // EXPECT_EQ(kodo::write_feedback(encoder), 0U);
    }

}
