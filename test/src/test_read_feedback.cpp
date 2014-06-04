// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_read_feedback.cpp Unit tests for the read_feedback() function

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/read_feedback.hpp>
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
            void read_feedback(const uint8_t* ptr)
            {
                m_ptr = ptr;
            }

            const uint8_t* m_ptr;
        };

        struct dummy_two
        {
            const uint8_t* m_ptr;
        };

    }
}

TEST(TestReadFeedback, invoke)
{

    uint8_t ptr = 0;

    {
        // Check with a standard object
        kodo::dummy_one d;
        kodo::read_feedback(d, &ptr);
        EXPECT_EQ(d.m_ptr, &ptr);
    }

    {
        // Check that the code compiles even with an object that
        // does not have the read_feedback function
        kodo::dummy_two d;

        if(kodo::has_read_feedback<kodo::dummy_two>::value)
        {
            kodo::read_feedback(d, &ptr);
            EXPECT_EQ(d.m_ptr, &ptr);
        }
    }

    {
        // Check that the code compiles with a normal stack
        // typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

        // encoder_type::factory factory(10,10);
        // auto encoder = factory.build();

        // EXPECT_EQ(kodo::read_feedback(encoder), 0U);
    }

}
