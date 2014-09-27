// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_read_feedback.cpp Unit tests for the
///       has_read_feedback class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_read_feedback.hpp>
#include <kodo/rlnc/sliding_window_encoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy
        {
            void read_feedback(const uint8_t*);
        };

        struct dummy_parent : public dummy
        { };

        struct dummy_false
        { };

    }
}

TEST(TestHasRead_Feedback, detect)
{
    EXPECT_FALSE(kodo::has_read_feedback<uint32_t>::value);
    EXPECT_FALSE(kodo::has_read_feedback<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_read_feedback<kodo::dummy>::value);
    EXPECT_TRUE(kodo::has_read_feedback<kodo::dummy_parent>::value);

    typedef kodo::sliding_window_encoder<fifi::binary8> encoder_type;
    EXPECT_TRUE(kodo::has_read_feedback<encoder_type>::value);
}
