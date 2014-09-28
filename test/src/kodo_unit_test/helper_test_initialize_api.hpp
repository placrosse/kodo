// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_rlnc_on_the_fly_codes.cpp Unit tests for the full
///       vector codes (i.e. Network Coding encoders and decoders).

#include <ctime>

#include <gtest/gtest.h>

/// Checks that the encoders and decoders are in a clean state after using
/// the initialize function.
template<class Encoder, class Decoder>
inline void run_test_initialize(uint32_t symbols, uint32_t symbol_size)
{
    // Common setting
    kodo::basic_factory<Encoder> encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    kodo::basic_factory<Decoder> decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    for (uint32_t i = 0; i < 3; ++i)
    {
        encoder->initialize(encoder_factory);
        decoder->initialize(decoder_factory);

        std::vector<uint8_t> payload(encoder->payload_size());

        // Ensure that the we may re-use the encoder also with partial
        // data. We allow one partial symbol so by subtracting at
        // maximum the size of one symbol we will get at most one
        // partial symbol :)
        uint32_t block_size = encoder->block_size() -
            (rand() % encoder->symbol_size());

        std::vector<uint8_t> data_in = random_vector(block_size);
        std::vector<uint8_t> data_out(decoder->block_size(), '\0');

        encoder->set_symbols(sak::storage(data_in));

        // If the decoder uses shallow storage we have to initialize
        // it's decoding buffers
        if (kodo::has_shallow_symbol_storage<Decoder>::value)
        {
            decoder->set_symbols(sak::storage(data_out));
        }

        // Set the encoder non-systematic
        if (kodo::has_systematic_encoder<Encoder>::value)
        {
            kodo::set_systematic_off(encoder);
        }

        while (!decoder->is_complete())
        {
            uint32_t payload_used = encoder->encode( &payload[0] );
            EXPECT_TRUE(payload_used <= encoder->payload_size());

            decoder->decode( &payload[0] );
        }

        // If the decoder uses deep storage we need to copy out the
        // decoded data
        if (kodo::has_deep_symbol_storage<Decoder>::value)
        {
            decoder->copy_symbols(sak::storage(data_out));
        }

        // Adjust the size of data_out since it may be larger than
        // data_in std::vector::resize should be fine, since it just
        // drops the excess bytes at the end.
        EXPECT_TRUE(data_in.size() <= data_out.size());
        data_out.resize(data_in.size());

        EXPECT_TRUE(data_out == data_in);
    }

}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_initialize(uint32_t symbols, uint32_t symbol_size)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbols_size = " << symbol_size);

    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_initialize
            <
            Encoder<fifi::binary>,
            Decoder<fifi::binary>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_initialize
            <
            Encoder<fifi::binary4>,
            Decoder<fifi::binary4>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_initialize
            <
            Encoder<fifi::binary8>,
            Decoder<fifi::binary8>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_initialize
            <
            Encoder<fifi::binary16>,
            Decoder<fifi::binary16>
            >(symbols, symbol_size);
    }
}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_initialize()
{
    test_initialize<Encoder, Decoder>(32, 1600);
    test_initialize<Encoder, Decoder>(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_initialize<Encoder, Decoder>(symbols, symbol_size);
}
