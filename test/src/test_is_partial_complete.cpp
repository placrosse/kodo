// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_partial_decoding_tracker.cpp Unit tests for the
///       has_partial_decoding_tracker class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/is_partial_complete.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/rlnc/on_the_fly_codes.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"

TEST(TestIsPartialComplete, check_false)
{
    typedef fifi::binary field_type;
    typedef kodo::full_rlnc_encoder<field_type> encoder_type;
    typedef kodo::full_rlnc_decoder<field_type> decoder_type;

    uint32_t symbols = 16;
    uint32_t symbol_size = 100;

    encoder_type::factory encoder_factory(symbols, symbol_size);
    decoder_type::factory decoder_factory(symbols, symbol_size);

    auto encoder = encoder_factory.build();
    auto decoder = decoder_factory.build();

    // Check that this code compiles

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<encoder_type>::value);
    if(kodo::has_partial_decoding_tracker<encoder_type>::value)
    {
        EXPECT_FALSE(kodo::is_partial_complete(encoder));
    }

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<decoder_type>::value);
    if(kodo::has_partial_decoding_tracker<decoder_type>::value)
    {
        EXPECT_FALSE(kodo::is_partial_complete(decoder));
    }
}

TEST(TestIsPartialComplete, check_true)
{
    typedef fifi::binary field_type;
    typedef kodo::on_the_fly_encoder<field_type> encoder_type;
    typedef kodo::on_the_fly_decoder<field_type> decoder_type;

    uint32_t symbols = 16;
    uint32_t symbol_size = 100;

    encoder_type::factory encoder_factory(symbols, symbol_size);
    decoder_type::factory decoder_factory(symbols, symbol_size);

    auto encoder = encoder_factory.build();
    auto decoder = decoder_factory.build();
    auto recoder = decoder_factory.build();

    // Check that this code compiles

    EXPECT_FALSE(kodo::has_partial_decoding_tracker<encoder_type>::value);
    if(kodo::has_partial_decoding_tracker<encoder_type>::value)
    {
        EXPECT_FALSE(kodo::is_partial_complete(encoder));
    }

    EXPECT_TRUE(kodo::has_partial_decoding_tracker<decoder_type>::value);
    if(kodo::has_partial_decoding_tracker<decoder_type>::value)
    {
        EXPECT_FALSE(kodo::is_partial_complete(decoder));
    }

    std::vector<uint8_t> payload(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    while( !decoder->is_complete() )
    {
        encoder->encode( &payload[0] );

        // Simulate some loss
        if((rand() % 2) == 0)
            continue;

        // Use recoder in some cases
        if((rand() % 2) == 0)
        {
            recoder->decode( &payload[0] );
            recoder->recode( &payload[0] );
        }

        decoder->decode( &payload[0] );

        // set symbol 50% of the time ONLY if rank is not full
        if(((rand() % 2) == 0) && (encoder->rank() < symbols))
        {
            uint32_t i = encoder->rank();
            encoder->set_symbol(i, symbol_sequence[i]);
        }

        // Check that the call works
        bool ok = kodo::is_partial_complete(decoder);

        if(ok)
        {
            // Check that we as many pivot elements as expected and that these
            // are decoded
            uint32_t symbols_uncoded = 0;
            for(uint32_t i = 0; i < decoder->symbols(); ++i)
            {
                if(!decoder->is_symbol_uncoded(i))
                    continue;

                ++symbols_uncoded;

                auto symbol_storage =
                    sak::storage(decoder->symbol(i), decoder->symbol_size());

                EXPECT_TRUE(sak::is_equal(symbol_storage, symbol_sequence[i]));
            }

            EXPECT_EQ(symbols_uncoded, decoder->symbols_uncoded());

        }
    }

    EXPECT_TRUE(kodo::is_partial_complete(decoder));

    std::vector<uint8_t> data_out(decoder->block_size(), '\0');
    decoder->copy_symbols(sak::storage(data_out));

    EXPECT_TRUE(std::equal(data_out.begin(),
                           data_out.end(),
                           data_in.begin()));
}


TEST(TestIsPartialComplete, check_two_encoders)
{
    typedef fifi::binary field_type;
    typedef kodo::on_the_fly_encoder<field_type> encoder_type;
    typedef kodo::on_the_fly_decoder<field_type> decoder_type;

    uint32_t symbols = 16;
    uint32_t symbol_size = 100;

    encoder_type::factory encoder_factory(symbols, symbol_size);
    decoder_type::factory decoder_factory(symbols, symbol_size);

    auto encoder_one = encoder_factory.build();
    auto encoder_two = encoder_factory.build();
    auto decoder = decoder_factory.build();

    std::vector<uint8_t> payload(encoder_one->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder_one->block_size());

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    while( !decoder->is_complete() )
    {

        // Alternate between the encoders
        if((rand() % 2) == 0)
        {
            encoder_one->encode( &payload[0] );
        }
        else
        {
            encoder_two->encode( &payload[0] );
        }

        // Simulate some loss
        if((rand() % 2) == 0)
            continue;

        decoder->decode( &payload[0] );

        // set symbol 50% of the time ONLY if rank is not full
        if(((rand() % 2) == 0) && (encoder_one->rank() < symbols))
        {
            uint32_t i = encoder_one->rank();
            encoder_one->set_symbol(i, symbol_sequence[i]);
        }

        // set symbol 50% of the time ONLY if rank is not full
        if(((rand() % 2) == 0) && (encoder_two->rank() < symbols))
        {
            uint32_t i = encoder_two->rank();
            encoder_two->set_symbol(i, symbol_sequence[i]);
        }

        // Check that the call works
        bool ok = kodo::is_partial_complete(decoder);

        if(ok)
        {
            // Check that we as many decoded symbols as expected and that these
            // are decoded
            uint32_t symbols_uncoded = 0;
            for(uint32_t i = 0; i < decoder->symbols(); ++i)
            {
                if(!decoder->is_symbol_uncoded(i))
                    continue;

                ++symbols_uncoded;

                auto symbol_storage =
                    sak::storage(decoder->symbol(i), decoder->symbol_size());

                EXPECT_TRUE(sak::is_equal(symbol_storage, symbol_sequence[i]));
            }

            EXPECT_EQ(symbols_uncoded, decoder->symbols_uncoded());

        }


    }

    EXPECT_TRUE(kodo::is_partial_complete(decoder));

    std::vector<uint8_t> data_out(decoder->block_size(), '\0');
    decoder->copy_symbols(sak::storage(data_out));

    EXPECT_TRUE(std::equal(data_out.begin(),
                           data_out.end(),
                           data_in.begin()));
}
