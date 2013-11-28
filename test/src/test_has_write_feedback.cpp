// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_write_feedback.cpp Unit tests for the
///       has_write_feedback class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_write_feedback.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy
        {
            uint32_t write_feedback(uint8_t*);
        };

        struct dummy_parent : public dummy
        { };

        struct dummy_false
        { };

    }
}

TEST(TestHasWriteFeedback, detect)
{
    EXPECT_FALSE(kodo::has_write_feedback<uint32_t>::value);
    EXPECT_FALSE(kodo::has_write_feedback<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_write_feedback<kodo::dummy>::value);
    EXPECT_TRUE(kodo::has_write_feedback<kodo::dummy_parent>::value);

    typedef kodo::sliding_window_decoder<fifi::binary8> decoder_type;
    EXPECT_TRUE(kodo::has_write_feedback<decoder_type>::value);
}



