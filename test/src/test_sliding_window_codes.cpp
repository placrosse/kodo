// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_sliding_window_codes.cpp Unit tests for the full
///       vector codes (i.e. Network Coding encoders and decoders).

#include <ctime>

#include <gtest/gtest.h>
#include <kodo/rlnc/sliding_window_encoder.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"

#include "kodo_unit_test/helper_test_reuse_api.hpp"
#include "kodo_unit_test/helper_test_recoding_api.hpp"
#include "kodo_unit_test/helper_test_on_the_fly_api.hpp"
#include "kodo_unit_test/helper_test_basic_api.hpp"
#include "kodo_unit_test/helper_test_initialize_api.hpp"
#include "kodo_unit_test/helper_test_systematic_api.hpp"
#include "kodo_unit_test/helper_test_mix_uncoded_api.hpp"

// Most of the unit-test helpers only accept codecs which have one
// undefined template arguemnt, namely the field (this does not work
// eventhough the other arguments to a codec stack have defaults).
namespace
{
    template<class Field>
    class decoder : public kodo::sliding_window_decoder<
        Field, kodo::disable_trace>
    { };

    template<class Field>
    class encoder : public kodo::sliding_window_encoder<
        Field>
    { };

}

// Tests the basic API functionality this mean basic encoding
// and decoding
TEST(TestSlidingWindowCodes, test_basic_api)
{
    test_basic_api<encoder, decoder>();
}

// Test that the encoders and decoders initialize() function can be used
// to reset the state of an encoder and decoder and that they therefore
// can be safely reused.
TEST(TestSlidingWindowCodes, test_initialize)
{
    test_initialize<encoder, decoder>();
}

// Tests that an encoder producing systematic packets is handled
// correctly in the decoder.
TEST(TestSlidingWindowCodes, test_systematic)
{
    test_systematic<encoder, decoder>();
}

// Tests whether mixed un-coded and coded packets are correctly handled
// in the encoder and decoder.
TEST(TestSlidingWindowCodes, mix_uncoded)
{
    test_mix_uncoded<encoder, decoder>();
}

// The recoding
TEST(TestSlidingWindowCodes, test_recoders_api)
{
    test_recoders<encoder, decoder>();
}

// The recoding
TEST(TestSlidingWindowCodes, test_recoding_relay)
{
    test_recoding_relay<encoder, decoder>();
}

// Tests the basic API functionality this mean basic encoding
// and decoding
TEST(TestSlidingWindowCodes, test_reuse_api)
{
    test_reuse<encoder, decoder>();
}

// Tests the basic API functionality this mean basic encoding
// and decoding
TEST(TestSlidingWindowCodes, test_reuse_incomplete_api)
{
    test_reuse_incomplete<encoder, decoder>();
}
