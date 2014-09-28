// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_rlnc_on_the_fly_codes.cpp Unit tests for the full
///       vector codes (i.e. Network Coding encoders and decoders).

#include <ctime>

#include <gtest/gtest.h>

template<class Encoder, class Decoder>
inline void run_test_mix_uncoded(uint32_t symbols, uint32_t symbol_size)
{
    // Common setting
    kodo::basic_factory<Encoder> encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    kodo::basic_factory<Decoder> decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    // Encode/decode operations
    EXPECT_TRUE(encoder->payload_size() == decoder->payload_size());

    uint32_t feedback_size = 0;

    if (kodo::has_write_feedback<Decoder>::value)
    {
        EXPECT_EQ(kodo::feedback_size(encoder),
                  kodo::feedback_size(decoder));

        feedback_size = kodo::feedback_size(encoder);
        EXPECT_TRUE(feedback_size > 0);
    }

    std::vector<uint8_t> feedback(feedback_size);

    std::vector<uint8_t> payload(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());
    std::vector<uint8_t> data_out(decoder->block_size(), '\0');

    // If the decoder uses shallow storage we have to initialize
    // it's decoding buffers
    if (kodo::has_shallow_symbol_storage<Decoder>::value)
    {
        decoder->set_symbols(sak::storage(data_out));
    }

    encoder->set_symbols(sak::storage(data_in));

    if (kodo::has_systematic_encoder<Encoder>::value)
    {
        kodo::set_systematic_off(encoder);
    }

    while (!decoder->is_complete())
    {

        if ((rand() % 100) > 50)
        {
            encoder->encode( &payload[0] );
            decoder->decode( &payload[0] );
        }
        else
        {
            uint32_t symbol_id = rand() % encoder->symbols();

            if (decoder->is_symbol_pivot(symbol_id))
            {
                continue;
            }

            ASSERT_TRUE(encoder->symbol_size() <= payload.size());

            encoder->copy_symbol(
                symbol_id, sak::storage(payload));

            const uint8_t *symbol_src = encoder->symbol(symbol_id);
            const uint8_t *symbol_dest = &payload[0];

            EXPECT_TRUE(std::equal(symbol_src,
                                   symbol_src + encoder->symbol_size(),
                                   symbol_dest));


            decoder->decode_symbol(&payload[0], symbol_id);

            if (kodo::has_write_feedback<Decoder>::value)
            {
                uint32_t written = kodo::write_feedback(decoder, &feedback[0]);
                EXPECT_TRUE(written > 0);

                // Pass to the encoder
                kodo::read_feedback(encoder, &feedback[0]);
            }

        }
    }

    EXPECT_EQ(encoder->block_size(), decoder->block_size());

    // If the decoder uses deep storage we need to copy out the
    // decoded data
    if (kodo::has_deep_symbol_storage<Decoder>::value)
    {
        decoder->copy_symbols(sak::storage(data_out));
    }

    EXPECT_TRUE(data_in == data_out);

}


template
<
    template <class...> class Encoder,
    template <class...> class Decoder
    >
inline void test_mix_uncoded(uint32_t symbols, uint32_t symbol_size)
{
    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_mix_uncoded
            <
            Encoder<fifi::binary>,
            Decoder<fifi::binary>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_mix_uncoded
            <
            Encoder<fifi::binary4>,
            Decoder<fifi::binary4>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_mix_uncoded
            <
            Encoder<fifi::binary8>,
            Decoder<fifi::binary8>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_mix_uncoded
            <
            Encoder<fifi::binary16>,
            Decoder<fifi::binary16>
            >(symbols, symbol_size);
    }
}

template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_mix_uncoded()
{
    test_mix_uncoded<Encoder,Decoder>(32, 1600);
    test_mix_uncoded<Encoder,Decoder>(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_mix_uncoded<Encoder, Decoder>(symbols, symbol_size);
}
