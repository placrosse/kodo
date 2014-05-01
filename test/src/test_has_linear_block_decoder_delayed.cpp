// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_linear_block_decoder_delayed.cpp Unit tests for the
///       has_linear_block_decoder_delayed class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_linear_block_decoder_delayed.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/rlnc/on_the_fly_codes.hpp>

namespace
{
    template<class Field>
    using decoder = kodo::full_rlnc_decoder<Field, kodo::disable_trace>;

    template<class Field>
    using encoder = kodo::full_rlnc_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using shallow_delayed_decoder =
        kodo::shallow_delayed_full_rlnc_decoder<Field, kodo::disable_trace>;
}

TEST(TestHasLinearBlockDecoderDelayed, detect)
{
    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     encoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     encoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     decoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     decoder<fifi::binary8> >::value);

    EXPECT_TRUE(kodo::has_linear_block_decoder_delayed<
                    shallow_delayed_decoder<fifi::binary> >::value);

    EXPECT_TRUE(kodo::has_linear_block_decoder_delayed<
                    shallow_delayed_decoder<fifi::binary8> >::value);
}
