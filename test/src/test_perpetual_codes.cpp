// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing


/// @file test_perpetual_codes.cpp Unit tests for the perpetual code

#include <ctime>

#include <gtest/gtest.h>

#include <kodo/rlnc/perpetual_codes.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"
//#include "kodo_unit_test/helper_test_reuse_api.hpp"
//#include "kodo_unit_test/helper_test_recoding_api.hpp"
// #include "kodo_unit_test/helper_test_on_the_fly_api.hpp"
#include "kodo_unit_test/helper_test_basic_api.hpp"
#include "kodo_unit_test/helper_test_initialize_api.hpp"
// #include "kodo_unit_test/helper_test_systematic_api.hpp"
//#include "kodo_unit_test/helper_test_mix_uncoded_api.hpp"

namespace
{
    template<class Field>
    using encoder = kodo::perpetual_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using decoder = kodo::perpetual_decoder<Field, kodo::disable_trace>;

//     template<class Field>
//     using shallow_encoder =
//         kodo::shallow_full_rlnc_encoder<Field, kodo::disable_trace>;
//
//     template<class Field>
//     using shallow_decoder =
//         kodo::shallow_full_rlnc_decoder<Field, kodo::disable_trace>;
}


/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestPerpetualCodes, test_basic_api)
{
    test_basic_api<encoder, decoder>();
}

/// Test that the encoders and decoders initialize() function can be used
/// to reset the state of an encoder and decoder and that they therefore
/// can be safely reused.
TEST(TestPerpetualCodes, test_initialize)
{
    test_initialize<encoder, decoder>();
}

/// Tests that an encoder producing systematic packets is handled
/// correctly in the decoder.
// TEST(TestPerpetualCodes, test_systematic)
// {
    // test_systematic<encoder, kodo::full_rlnc_decoder>();
    // test_systematic<encoder, decoder>();
// }

/// Tests whether mixed un-coded and coded packets are correctly handled
/// in the encoder and decoder.
// TEST(TestPerpetualCodes, mix_uncoded)
// {
//     test_mix_uncoded<encoder, decoder>();
// }
//
// /// The recoding
// TEST(TestPerpetualCodes, test_recoders_api)
// {
//     test_recoders<encoder, decoder>();
// }
//
// /// The recoding
// TEST(TestPerpetualCodes, test_recoding_relay)
// {
//     test_recoding_relay<encoder, decoder>();
// }
//
// /// Tests the basic API functionality this mean basic encoding and decoding
// TEST(TestPerpetualCodes, test_reuse_api)
// {
//     test_reuse<encoder, decoder>();
// }
//
// /// Tests the basic API functionality this mean basic encoding and decoding
// TEST(TestPerpetualCodes, test_reuse_incomplete_api)
// {
//     test_reuse_incomplete<encoder, decoder>();
// }
