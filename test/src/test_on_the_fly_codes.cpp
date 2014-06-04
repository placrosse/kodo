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

/// Unit test added in response to a bug report regarding the remote_rank().
///
/// A bug in the proxy_remote_rank layer meant that the it when
/// initialized it would use the main stack pointer of the previously
/// built decoder. In the code below both decoder_1 and decoder_2
/// would use decoder_3 as main stack when initialized again.
///
/// This means that when decoder_1 recodes it will put the remote_rank
/// of decoder_3 in the packet which we in this unit test makes sure is full.
TEST(TestOnTheFlyCodes, test_recode_remote_rank)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 1600;

    encoder<fifi::binary>::factory encoder_factory(symbols, symbol_size);
    decoder<fifi::binary>::factory decoder_factory(symbols, symbol_size);

    auto encoder = encoder_factory.build();
    auto decoder_1 = decoder_factory.build();
    auto decoder_2 = decoder_factory.build();
    auto decoder_3 = decoder_factory.build();

    std::vector<uint8_t> data_in = random_vector(encoder->block_size());
    std::vector<uint8_t> payload(encoder->payload_size());

    encoder->set_symbols(sak::storage(data_in));

    while(!decoder_3->is_complete())
    {
        encoder->encode(payload.data());
        decoder_3->decode(payload.data());
    }

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    encoder->initialize(encoder_factory);
    decoder_1->initialize(decoder_factory);
    decoder_2->initialize(decoder_factory);

    // Only set the first symbol on the encoder
    encoder->set_symbol(0, symbol_sequence[0]);
    EXPECT_EQ(encoder->rank(), 1U);

    encoder->encode(payload.data());

    decoder_1->decode(payload.data());
    decoder_1->recode(payload.data());
    EXPECT_EQ(decoder_1->rank(), 1U);
    EXPECT_EQ(decoder_1->remote_rank(), 1U);

    decoder_2->decode(payload.data());

    // In presence of the bug the remote_rank here is 10
    EXPECT_EQ(decoder_2->remote_rank(), 1U);
}
