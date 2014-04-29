// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <fifi/binary.hpp>
#include <fifi/binary4.hpp>
#include <fifi/binary8.hpp>
#include <fifi/binary16.hpp>
#include <fifi/prime2325.hpp>
#include <fifi/is_prime2325.hpp>
#include <fifi/prime2325_binary_search.hpp>
#include <fifi/prime2325_apply_prefix.hpp>

#include <kodo/systematic_encoder.hpp>

#include "basic_api_test_helper.hpp"

#include <kodo/has_systematic_encoder.hpp>
#include <kodo/set_systematic_off.hpp>
#include <kodo/write_feedback.hpp>
#include <kodo/read_feedback.hpp>
#include <kodo/feedback_size.hpp>
#include <kodo/set_systematic_off.hpp>

#include <kodo/has_rank.hpp>
#include <kodo/rank.hpp>
#include <kodo/proxy_args.hpp>

/// Helper function which will invoke a number of checks on an encoder
/// and decoder pair
template<class Encoder, class Decoder>
inline void run_test_basic_api(uint32_t symbols, uint32_t symbol_size)
{

    // Common setting
    typename Encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    typename Decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    EXPECT_TRUE(symbols == encoder_factory.max_symbols());
    EXPECT_TRUE(symbol_size == encoder_factory.max_symbol_size());
    EXPECT_TRUE(symbols == encoder->symbols());
    EXPECT_TRUE(symbol_size == encoder->symbol_size());

    EXPECT_TRUE(symbols == decoder_factory.max_symbols());
    EXPECT_TRUE(symbol_size == decoder_factory.max_symbol_size());
    EXPECT_TRUE(symbols == decoder->symbols());
    EXPECT_TRUE(symbol_size == decoder->symbol_size());

    EXPECT_TRUE(encoder->symbol_length() > 0);
    EXPECT_TRUE(decoder->symbol_length() > 0);

    EXPECT_TRUE(encoder->block_size() == symbols * symbol_size);
    EXPECT_TRUE(decoder->block_size() == symbols * symbol_size);

    EXPECT_TRUE(encoder_factory.max_payload_size() >=
                encoder->payload_size());

    EXPECT_TRUE(decoder_factory.max_payload_size() >=
                decoder->payload_size());

    EXPECT_EQ(encoder_factory.max_payload_size(),
              decoder_factory.max_payload_size());

    // Encode/decode operations
    EXPECT_EQ(encoder->payload_size(), decoder->payload_size());

    EXPECT_EQ((uint32_t)kodo::has_write_feedback<Decoder>::value,
              (uint32_t)kodo::has_read_feedback<Encoder>::value);

    uint32_t feedback_size = 0;

    if(kodo::has_write_feedback<Decoder>::value)
    {
        EXPECT_EQ(kodo::feedback_size(encoder),
                  kodo::feedback_size(decoder));

        feedback_size = kodo::feedback_size(encoder);
        EXPECT_TRUE(feedback_size > 0);
    }

    std::vector<uint8_t> feedback(feedback_size);

    std::vector<uint8_t> payload(encoder->payload_size());

    std::vector<uint8_t> data_in = random_vector(encoder->block_size());
    std::vector<uint8_t> data_in_copy(data_in);

    sak::mutable_storage storage_in = sak::storage(data_in);
    sak::mutable_storage storage_in_copy = sak::storage(data_in_copy);

    EXPECT_TRUE(sak::equal(storage_in, storage_in_copy));

    // Only used for prime fields, lets reconsider how we implement
    // this less intrusive
    uint32_t prefix = 0;

    if(fifi::is_prime2325<typename Encoder::field_type>::value)
    {
        // This field only works for multiple of uint32_t
        assert((encoder->block_size() % 4) == 0);

        uint32_t block_length =
            fifi::size_to_length<fifi::prime2325>(encoder->block_size());

        fifi::prime2325_binary_search search(block_length);
        prefix = search.find_prefix(storage_in_copy);

        // Apply the negated prefix
        fifi::apply_prefix(storage_in_copy, ~prefix);
    }

    encoder->set_symbols(storage_in_copy);

    // Set the encoder non-systematic
   if(kodo::has_systematic_encoder<Encoder>::value)
       kodo::set_systematic_off(encoder);

    while( !decoder->is_complete() )
    {
        uint32_t payload_used = encoder->encode( &payload[0] );
        EXPECT_TRUE(payload_used <= encoder->payload_size());

        decoder->decode( &payload[0] );

        if(kodo::has_write_feedback<Decoder>::value)
        {
            uint32_t written = kodo::write_feedback(decoder, &feedback[0]);
            EXPECT_TRUE(written > 0);

            // Pass to the encoder
            kodo::read_feedback(encoder, &feedback[0]);
        }

    }

    std::vector<uint8_t> data_out(decoder->block_size(), '\0');
    decoder->copy_symbols(sak::storage(data_out));

    if(fifi::is_prime2325<typename Encoder::field_type>::value)
    {
        // Now we have to apply the negated prefix to the decoded data
        fifi::apply_prefix(sak::storage(data_out), ~prefix);
    }

    EXPECT_TRUE(data_out == data_in);
}

/// Helper function that invokes the test_basic_api using a number of
/// different finite fields
template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_basic_api(uint32_t symbols, uint32_t symbol_size)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbol_size = " << symbols);

    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_basic_api
            <
                Encoder<fifi::binary>,
                Decoder<fifi::binary>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_basic_api
            <
                Encoder<fifi::binary4>,
                Decoder<fifi::binary4>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_basic_api
            <
                Encoder<fifi::binary8>,
                Decoder<fifi::binary8>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_basic_api
            <
                Encoder<fifi::binary16>,
                Decoder<fifi::binary16>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = prime2325");
        run_test_basic_api
            <
                Encoder<fifi::prime2325>,
                Decoder<fifi::prime2325>
            >(symbols, symbol_size);
    }
}

/// Helper function that invokes the test_basic_api functions using a
/// set of symbols and symbol sizes
template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_basic_api()
{

    test_basic_api<Encoder, Decoder>(32, 1600);
    test_basic_api<Encoder, Decoder>(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_basic_api<Encoder, Decoder>(symbols, symbol_size);
}
