// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_set_systematic_on.cpp Unit tests for the
///       set_systematic_on function

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/set_systematic_on.hpp>
#include <kodo/set_systematic_off.hpp>
#include <kodo/has_set_systematic_on.hpp>
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
            void set_systematic_on()
            {
                m_systematic_on = true;
            }

            bool m_systematic_on;
        };

        struct dummy_two
        { };

    }
}

TEST(TestSetSystematicOn, set)
{

    {
        // Check with a standard object
        kodo::dummy_one d;
        d.m_systematic_on = false;

        kodo::set_systematic_on(d);
        EXPECT_TRUE(d.m_systematic_on);
    }

    {
        // Check that the code compiles even with an object that
        // does not have the set_systematic_on function
        kodo::dummy_two d;

        if(kodo::has_set_systematic_on<kodo::dummy_two>::value)
        {
            kodo::set_systematic_on(d);
        }
    }

    {
        // Check that the code compiles with a normal stack
        typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

        encoder_type::factory factory(10,10);
        auto encoder = factory.build();

        EXPECT_TRUE(encoder->is_systematic_on());

        kodo::set_systematic_on(encoder);
        EXPECT_TRUE(encoder->is_systematic_on());

        kodo::set_systematic_off(encoder);
        EXPECT_FALSE(encoder->is_systematic_on());

        kodo::set_systematic_on(encoder);
        EXPECT_TRUE(encoder->is_systematic_on());


    }

}
