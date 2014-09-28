// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_rlnc_on_the_fly_codes.cpp Unit tests for the full
///       vector codes (i.e. Network Coding encoders and decoders).

#include <ctime>

#include <gtest/gtest.h>

template<class Encoder, class Decoder>
inline void run_test_systematic(uint32_t symbols, uint32_t symbol_size)
{
    // Common setting
    kodo::basic_factory<Encoder> encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    kodo::basic_factory<Decoder> decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    // Encode/decode operations
    EXPECT_TRUE(encoder->payload_size() == decoder->payload_size());

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

    // Ensure encoder systematic
    ASSERT_TRUE(kodo::has_systematic_encoder<Encoder>::value);
    kodo::set_systematic_on(encoder);

    uint32_t pkg_count = 0;

    while (!decoder->is_complete())
    {
        encoder->encode( &payload[0] );
        decoder->decode( &payload[0] );

        ++pkg_count;
    }

    EXPECT_TRUE(pkg_count == encoder->symbols());

    // If the decoder uses deep storage we need to copy out the
    // decoded data
    if (kodo::has_deep_symbol_storage<Decoder>::value)
    {
        decoder->copy_symbols(sak::storage(data_out));
    }

    EXPECT_TRUE(data_out == data_in);

}

template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_systematic(uint32_t symbols, uint32_t symbol_size)
{
    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_systematic
            <
            Encoder<fifi::binary>,
            Decoder<fifi::binary>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_systematic
            <
            Encoder<fifi::binary4>,
            Decoder<fifi::binary4>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_systematic
            <
            Encoder<fifi::binary8>,
            Decoder<fifi::binary8>
            >(symbols, symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_systematic
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
inline void test_systematic()
{
    test_systematic<Encoder, Decoder>(32, 1600);
    test_systematic<Encoder, Decoder>(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_systematic<Encoder, Decoder>(symbols, symbol_size);
}
