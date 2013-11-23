// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing


/// @file test_sliding_window_codes.cpp Unit tests for the full
///       vector codes (i.e. Network Coding encoders and decoders).

#include <ctime>

#include <gtest/gtest.h>
#include <kodo/rlnc/sliding_window_encoder.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>

#include "basic_api_test_helper.hpp"

#include "helper_test_reuse_api.hpp"
#include "helper_test_recoding_api.hpp"
#include "helper_test_on_the_fly_api.hpp"
#include "helper_test_basic_api.hpp"
#include "helper_test_initialize_api.hpp"
#include "helper_test_systematic_api.hpp"
#include "helper_test_mix_uncoded_api.hpp"

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestSlidingWindowCodes, test_basic_api)
{
    test_basic_api<kodo::sliding_window_encoder,
        kodo::sliding_window_decoder>();
}



// /// Test that the encoders and decoders initialize() function can be used
// /// to reset the state of an encoder and decoder and that they therefore
// /// can be safely reused.
// TEST(TestRlncFullVectorCodes, test_initialize)
// {
//     test_initialize<kodo::full_rlnc_encoder_shallow,
//         kodo::full_rlnc_decoder>();

//     test_initialize<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder>();

//     // The delayed decoders
//     test_initialize<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed>();

// }

// /// Tests that an encoder producing systematic packets is handled
// /// correctly in the decoder.
// TEST(TestRlncFullVectorCodes, test_systematic)
// {
//     test_systematic<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder>();

//     test_systematic<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed>();
// }

// /// Tests whether mixed un-coded and coded packets are correctly handled
// /// in the encoder and decoder.
// TEST(TestRlncFullVectorCodes, mix_uncoded)
// {
//     test_mix_uncoded<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder>();

//     test_mix_uncoded<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed>();
// }

// /// The recoding
// TEST(TestRlncFullVectorCodes, test_recoders_api)
// {

//     test_recoders<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder>();

//     test_recoders<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed>();

//     test_recoders<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed_shallow>();

// }

// /// The recoding
// TEST(TestRlncFullVectorCodes, test_recoding_relay)
// {

//     test_recoding_relay<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder>();

//     test_recoding_relay<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed>();

//     test_recoding_relay<kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed_shallow>();

// }

/// Tests the basic API functionality this mean basic encoding
/// and decoding
// TEST(TestRlncFullVectorCodes, test_reuse_api)
// {
//     test_reuse<
//         kodo::full_rlnc_encoder_shallow,
//         kodo::full_rlnc_decoder>();

//     test_reuse<
//         kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder>();

//     // The delayed decoders
//     test_reuse<
//         kodo::full_rlnc_encoder,
//         kodo::full_rlnc_decoder_delayed>();
// }

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestSlidingWindowCodes, test_reuse_incomplete_api)
{
    test_reuse_incomplete<kodo::sliding_window_encoder,
        kodo::sliding_window_decoder>();
}



