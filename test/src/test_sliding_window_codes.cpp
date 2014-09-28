// Copyright Steinwurf ApS 2011.
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
    using encoder = kodo::sliding_window_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using decoder = kodo::sliding_window_decoder<Field, kodo::disable_trace>;
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

// Tests that when recoding we get a systematic packet
TEST(TestSlidingWindowCodes, test_systematic_recoding)
{
    uint32_t symbols = 16;
    uint32_t symbol_size = 1600;

    kodo::basic_factory<encoder<fifi::binary8>>
        encoder_factory(symbols, symbol_size);

    kodo::basic_factory<decoder<fifi::binary8>>
        decoder_factory(symbols, symbol_size);

    auto encoder = encoder_factory.build();
    auto recoder = decoder_factory.build();
    auto decoder = decoder_factory.build();

    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    // Set the two symbol on the encoder
    encoder->set_symbol(0, symbol_sequence[0]);
    encoder->set_symbol(1, symbol_sequence[1]);

    std::vector<uint8_t> payload(encoder->payload_size());

    // The first call to encode should deliver the first symbol uncoded
    encoder->encode(payload.data());

    recoder->decode(payload.data());
    EXPECT_EQ(recoder->symbols_uncoded(), 1U);

    recoder->recode(payload.data());
    decoder->decode(payload.data());

    EXPECT_EQ(decoder->symbols_uncoded(), 1U);

    // The second call should do deliver the next
    encoder->encode(payload.data());

    recoder->decode(payload.data());
    EXPECT_EQ(recoder->symbols_uncoded(), 2U);

    recoder->recode(payload.data());
    decoder->decode(payload.data());

    EXPECT_EQ(decoder->symbols_uncoded(), 2U);



}
