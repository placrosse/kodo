// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <ctime>

#include <gtest/gtest.h>

#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/recoding_symbol_id.hpp>
#include <kodo/proxy_layer.hpp>
#include <kodo/payload_recoder.hpp>
#include <kodo/partial_shallow_symbol_storage.hpp>
#include <kodo/linear_block_decoder_delayed.hpp>
#include <kodo/shallow_symbol_storage.hpp>
#include <kodo/has_shallow_symbol_storage.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"

#include "kodo_unit_test/helper_test_reuse_api.hpp"
#include "kodo_unit_test/helper_test_recoding_api.hpp"
#include "kodo_unit_test/helper_test_on_the_fly_api.hpp"
#include "kodo_unit_test/helper_test_basic_api.hpp"
#include "kodo_unit_test/helper_test_initialize_api.hpp"
#include "kodo_unit_test/helper_test_systematic_api.hpp"
#include "kodo_unit_test/helper_test_mix_uncoded_api.hpp"

/// @todo rename this file like the sliding window files

namespace
{
    template<class Field>
    using decoder = kodo::full_rlnc_decoder<Field, kodo::disable_trace>;

    template<class Field>
    using encoder = kodo::full_rlnc_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using shallow_encoder =
        kodo::shallow_full_rlnc_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using shallow_delayed_decoder =
        kodo::shallow_delayed_full_rlnc_decoder<Field, kodo::disable_trace>;
}

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestRlncFullVectorCodes, test_basic_api)
{
    test_basic_api<shallow_encoder, decoder>();
    test_basic_api<encoder, decoder>();
    test_basic_api<encoder, shallow_delayed_decoder>();
}

/// Test that the encoders and decoders initialize() function can be used
/// to reset the state of an encoder and decoder and that they therefore
/// can be safely reused.
TEST(TestRlncFullVectorCodes, test_initialize)
{
    test_initialize<shallow_encoder, decoder>();
    test_initialize<encoder, decoder>();
    test_initialize<encoder, shallow_delayed_decoder>();
}

/// Tests that an encoder producing systematic packets is handled
/// correctly in the decoder.
TEST(TestRlncFullVectorCodes, test_systematic)
{
    test_systematic<shallow_encoder,decoder>();
    test_systematic<encoder, decoder>();
    test_systematic<encoder, shallow_delayed_decoder>();
}

/// Tests whether mixed un-coded and coded packets are correctly handled
/// in the encoder and decoder.
TEST(TestRlncFullVectorCodes, mix_uncoded)
{
    test_mix_uncoded<shallow_encoder,decoder>();
    test_mix_uncoded<encoder,decoder>();
    test_mix_uncoded<encoder,shallow_delayed_decoder>();
}

/// The recoding
TEST(TestRlncFullVectorCodes, test_recoders_api)
{
    test_recoders<shallow_encoder,decoder>();
    test_recoders<encoder,decoder>();
    test_recoders<encoder,shallow_delayed_decoder>();
}

/// The recoding
TEST(TestRlncFullVectorCodes, test_recoding_relay)
{
    test_recoding_relay<shallow_encoder,decoder>();
    test_recoding_relay<encoder,decoder>();
    test_recoding_relay<encoder,shallow_delayed_decoder>();
}

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestRlncFullVectorCodes, test_reuse_api)
{
    test_reuse<shallow_encoder,decoder>();
    test_reuse<encoder,decoder>();
    test_reuse<encoder,shallow_delayed_decoder>();
}

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestRlncFullVectorCodes, test_reuse_incomplete_api)
{
    test_reuse_incomplete<shallow_encoder, decoder>();
    test_reuse_incomplete<encoder, decoder>();
    test_reuse_incomplete<encoder, shallow_delayed_decoder>();
}
