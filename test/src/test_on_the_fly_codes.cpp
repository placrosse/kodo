// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <ctime>

#include <gtest/gtest.h>

#include <kodo/rlnc/on_the_fly_codes.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"

#include "kodo_unit_test/helper_test_reuse_api.hpp"
#include "kodo_unit_test/helper_test_recoding_api.hpp"
#include "kodo_unit_test/helper_test_on_the_fly_api.hpp"
#include "kodo_unit_test/helper_test_basic_api.hpp"
#include "kodo_unit_test/helper_test_initialize_api.hpp"
#include "kodo_unit_test/helper_test_systematic_api.hpp"
#include "kodo_unit_test/helper_test_mix_uncoded_api.hpp"

/// Define the stacks we will test
namespace
{
    template<class Field>
    using encoder = kodo::on_the_fly_encoder<Field>;//, kodo::disable_trace>;

    template<class Field>
    using decoder = kodo::on_the_fly_decoder<Field>;//, kodo::disable_trace>;
}


/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestOnTheFlyCodes, test_basic_api)
{
    test_basic_api<encoder,decoder>();
}

/// Test that the encoders and decoders initialize() function can be used
/// to reset the state of an encoder and decoder and that they therefore
/// can be safely reused.
TEST(TestOnTheFlyCodes, test_initialize_api)
{
    test_initialize<encoder,decoder>();
}

/// Tests that an encoder producing systematic packets is handled
/// correctly in the decoder.
TEST(TestOnTheFlyCodes, test_systematic_api)
{
    test_systematic<encoder,decoder>();
}

/// Tests whether mixed un-coded and coded packets are correctly handled
/// in the encoder and decoder.
TEST(TestOnTheFlyCodes, mix_uncoded_api)
{
    test_mix_uncoded<encoder,decoder>();
}

/// Test the recoding functionality of the stack
TEST(TestOnTheFlyCodes, test_recoders_api)
{
    test_recoders<encoder,decoder>();
}

/// Test the recoding functionality of the stack
TEST(TestOnTheFlyCodes, test_recoding_relay)
{
    test_recoding_relay<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestOnTheFlyCodes, test_on_the_fly_api)
{
    test_on_the_fly<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestOnTheFlyCodes, test_on_the_fly_systematic_api)
{
    test_on_the_fly_systematic<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestOnTheFlyCodes, test_on_the_fly_systematic_no_errors_api)
{
    test_on_the_fly_systematic_no_errors<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestOnTheFlyCodes, test_reuse_api)
{
    test_reuse<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestOnTheFlyCodes, test_reuse_incomplete_api)
{
    test_reuse_incomplete<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestOnTheFlyCodes, test_systematic_packets_decode)
{
    test_systematic_packets_decode<encoder,decoder>();
}
