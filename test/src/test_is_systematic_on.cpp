// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_is_systematic_on.cpp Unit tests for the
///       is_systematic_on function

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/is_systematic_on.hpp>
#include <kodo/set_systematic_off.hpp>
#include <kodo/set_systematic_on.hpp>
#include <kodo/has_is_systematic_on.hpp>
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
            bool is_systematic_on() const
            {
                return m_systematic_on;
            }

            bool m_systematic_on;
        };

        struct dummy_two
        { };

    }
}

TEST(TestIsSystematicOn, set)
{

    {
        // Check with a standard object
        kodo::dummy_one d;
        d.m_systematic_on = false;

        EXPECT_FALSE(kodo::is_systematic_on(d));

        d.m_systematic_on = true;

        EXPECT_TRUE(kodo::is_systematic_on(d));

    }

    {
        // Check that the code compiles even with an object that
        // does not have the is_systematic_on function
        kodo::dummy_two d;

        if(kodo::has_is_systematic_on<kodo::dummy_two>::value)
        {
            bool v = kodo::is_systematic_on(d);

            // Avoid compiler warning about unused variable
            (void) v;
        }
    }

    {
        // Check that the code compiles with a normal stack
        typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

        encoder_type::factory factory(10,10);
        auto encoder = factory.build();

        EXPECT_TRUE(kodo::is_systematic_on(encoder));
        kodo::set_systematic_off(encoder);
        EXPECT_FALSE(kodo::is_systematic_on(encoder));
        kodo::set_systematic_on(encoder);
        EXPECT_TRUE(kodo::is_systematic_on(encoder));

    }

}



