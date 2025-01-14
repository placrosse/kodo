// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_partial_decoding_tracker.cpp Unit tests for the
///       has_partial_decoding_tracker class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_partial_decoding_tracker.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/rlnc/on_the_fly_codes.hpp>

TEST(TestHasPartialDecodingTracker, detect)
{
    EXPECT_FALSE(kodo::has_partial_decoding_tracker<
                     kodo::full_rlnc_encoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<
                     kodo::full_rlnc_encoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<
                     kodo::full_rlnc_decoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<
                     kodo::full_rlnc_decoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<
                     kodo::on_the_fly_encoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<
                     kodo::on_the_fly_encoder<fifi::binary8> >::value);

    EXPECT_TRUE(kodo::has_partial_decoding_tracker<
                     kodo::on_the_fly_decoder<fifi::binary> >::value);

    EXPECT_TRUE(kodo::has_partial_decoding_tracker<
                     kodo::on_the_fly_decoder<fifi::binary8> >::value);

}
