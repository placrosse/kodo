// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <ctime>

#include <gtest/gtest.h>
#include <kodo/rlnc/seed_rlnc_codes.hpp>

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
    using encoder = kodo::seed_rlnc_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using decoder = kodo::seed_rlnc_decoder<Field, kodo::disable_trace>;
}

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestSeedRlncCodes, test_basic_api)
{
    test_basic_api<encoder,decoder>();
}

/// Test that the encoders and decoders initialize() function can be used
/// to reset the state of an encoder and decoder and that they therefore
/// can be safely reused.
TEST(TestSeedRlncCodes, test_initialize_api)
{
    test_initialize<encoder,decoder>();
}

/// Tests that an encoder producing systematic packets is handled
/// correctly in the decoder.
TEST(TestSeedRlncCodes, test_systematic_api)
{
    test_systematic<encoder,decoder>();
}

/// Tests whether mixed un-coded and coded packets are correctly handled
/// in the encoder and decoder.
TEST(TestSeedRlncCodes, mix_uncoded_api)
{
    test_mix_uncoded<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestSeedRlncCodes, test_reuse_api)
{
    test_reuse<encoder,decoder>();
}

/// Tests that we can progressively set on symbol at-a-time on
/// encoder
TEST(TestSeedRlncCodes, test_reuse_incomplete_api)
{
    test_reuse_incomplete<encoder,decoder>();
}
