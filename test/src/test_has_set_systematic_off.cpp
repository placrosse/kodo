// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_set_systematic_off.cpp Unit tests for the
///       has_set_systematic_off class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_set_systematic_off.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy
        {
            void set_systematic_off();
        };

        struct dummy_parent : public dummy
        { };

        struct dummy_false
        { };

    }
}

TEST(TestHasSetSystematicOff, detect)
{
    EXPECT_FALSE(kodo::has_set_systematic_off<uint32_t>::value);
    EXPECT_FALSE(kodo::has_set_systematic_off<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_set_systematic_off<kodo::dummy>::value);
    EXPECT_TRUE(kodo::has_set_systematic_off<kodo::dummy_parent>::value);

    typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;
    EXPECT_TRUE(kodo::has_set_systematic_off<encoder_type>::value);
}
