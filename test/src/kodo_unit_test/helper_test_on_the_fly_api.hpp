// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing


/// @file helper_on_the_fly_api.hpp Unit test helper for the on the
///       fly API.  on-the-fly referrers to the fact the we
///       progressively will set symbols on the stack

#include <ctime>
#include <gtest/gtest.h>

#include <kodo/is_partial_complete.hpp>
#include <kodo/has_systematic_encoder.hpp>
#include <kodo/set_systematic_on.hpp>

/// Tests that an encoder support progressively specifying the symbols
template
<
    class Encoder,
    class Decoder
>
inline void run_test_on_the_fly(uint32_t symbols, uint32_t symbol_size)
{

    // Common setting
    typename Encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    typename Decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    std::vector<uint8_t> payload(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    // Set the encoder non-systematic
    if(kodo::has_systematic_encoder<Encoder>::value)
        kodo::set_systematic_off(encoder);

    EXPECT_EQ(encoder->rank(), 0U);
    EXPECT_EQ(decoder->rank(), 0U);

    while( !decoder->is_complete() )
    {
        EXPECT_TRUE(encoder->rank() >= decoder->rank());

        encoder->encode( &payload[0] );

        // Simulate some loss
        if((rand() % 2) == 0)
            continue;

        decoder->decode( &payload[0] );

        if(kodo::is_partial_complete(decoder))
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

        if(((rand() % 2) == 0) && encoder->rank() < symbols)
        {
            uint32_t i = encoder->rank();
            encoder->set_symbol(i, symbol_sequence[i]);
        }
    }

    std::vector<uint8_t> data_out(decoder->block_size(), '\0');
    decoder->copy_symbols(sak::storage(data_out));

    EXPECT_TRUE(std::equal(data_out.begin(),
                           data_out.end(),
                           data_in.begin()));
}


template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_on_the_fly(uint32_t symbols, uint32_t symbol_size)
{

    run_test_on_the_fly<
        Encoder<fifi::binary>,
        Decoder<fifi::binary> >(
        symbols, symbol_size);

    run_test_on_the_fly<
        Encoder<fifi::binary8>,
        Decoder<fifi::binary8> >(
        symbols, symbol_size);

    run_test_on_the_fly<
        Encoder<fifi::binary16>,
        Decoder<fifi::binary16> >(
        symbols, symbol_size);

}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_on_the_fly()
{

    test_on_the_fly<Encoder, Decoder>(32, 1600);
    test_on_the_fly<Encoder, Decoder>(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_on_the_fly<Encoder, Decoder>(symbols, symbol_size);

}


/// This class tests on-the-fly adding symbols to the encoding while
/// using the systematic feature of the encoder
template<class Encoder, class Decoder>
inline void run_test_on_the_fly_systematic(uint32_t symbols, uint32_t symbol_size)
{
    // Common setting
    typename Encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    typename Decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    // Encode/decode operations
    EXPECT_TRUE(encoder->payload_size() == decoder->payload_size());

    std::vector<uint8_t> payload(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    // Make sure the encoder is systematic
    if(kodo::has_systematic_encoder<Encoder>::value)
        kodo::set_systematic_on(encoder);


    while( !decoder->is_complete() )
    {
        encoder->encode( &payload[0] );

        // Simulate some loss
        if((rand() % 2) == 0)
            continue;

        uint32_t old_rank = decoder->rank();

        decoder->decode( &payload[0] );

        if(decoder->rank() > old_rank)
        {
            // We has a rank increase
            if(encoder->rank() == decoder->rank())
            {
                // The rank of the encoder matches the decoder, if
                // this unit test fails you most likely do not have a
                // layer which updates the decoding symbol status when
                // rank matches.  E.g. look at:
                // rank_symbol_decoding_status_updater.hpp
                EXPECT_TRUE(kodo::is_partial_complete(decoder));
            }
        }

        if(kodo::is_partial_complete(decoder))
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


        // set symbol 50% of the time ONLY if rank is not full
        if(((rand() % 2) == 0) && (encoder->rank() < symbols))
        {
            uint32_t i = encoder->rank();
            encoder->set_symbol(i, symbol_sequence[i]);
        }

        EXPECT_TRUE(encoder->rank() >= decoder->rank());
    }

    std::vector<uint8_t> data_out(decoder->block_size(), '\0');
    decoder->copy_symbols(sak::storage(data_out));

    EXPECT_TRUE(std::equal(data_out.begin(),
                           data_out.end(),
                           data_in.begin()));

}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_on_the_fly_systematic(uint32_t symbols, uint32_t symbol_size)
{
    run_test_on_the_fly_systematic
        <
        Encoder<fifi::binary>,
        Decoder<fifi::binary>
        >(symbols, symbol_size);

    run_test_on_the_fly_systematic
        <
        Encoder<fifi::binary8>,
        Decoder<fifi::binary8>
        >(symbols, symbol_size);

    run_test_on_the_fly_systematic
        <
        Encoder<fifi::binary16>,
        Decoder<fifi::binary16>
        >(symbols, symbol_size);
}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_on_the_fly_systematic()
{
    test_on_the_fly_systematic<Encoder, Decoder>(32, 1600);
    test_on_the_fly_systematic<Encoder, Decoder>(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_on_the_fly_systematic<Encoder, Decoder>(symbols, symbol_size);
}

/// This class tests on-the-fly adding symbols to the encoding while
/// using the systematic feature of the encoder
template<class Encoder, class Decoder>
inline void run_test_on_the_fly_systematic_no_errors(uint32_t symbols,
                                                 uint32_t symbol_size)
{
    // Common setting
    typename Encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    typename Decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    // Encode/decode operations
    EXPECT_TRUE(encoder->payload_size() == decoder->payload_size());

    std::vector<uint8_t> payload(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    // Make sure the encoder is systematic
    if(kodo::has_systematic_encoder<Encoder>::value)
        kodo::set_systematic_on(encoder);

    while( !decoder->is_complete() )
    {
        encoder->encode( &payload[0] );
        decoder->decode( &payload[0] );

        if(decoder->rank() > 0)
        {
            EXPECT_TRUE(kodo::is_partial_complete(decoder));
        }

        if(kodo::is_partial_complete(decoder))
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


        // set symbol 50% of the time ONLY if rank is not full
        if(encoder->rank() < symbols)
        {
            uint32_t i = encoder->rank();
            encoder->set_symbol(i, symbol_sequence[i]);
        }

        EXPECT_TRUE(encoder->rank() >= decoder->rank());
    }

    std::vector<uint8_t> data_out(decoder->block_size(), '\0');
    decoder->copy_symbols(sak::storage(data_out));

    EXPECT_TRUE(std::equal(data_out.begin(),
                           data_out.end(),
                           data_in.begin()));

}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_on_the_fly_systematic_no_errors(uint32_t symbols,
                                                 uint32_t symbol_size)
{
    run_test_on_the_fly_systematic_no_errors
        <
        Encoder<fifi::binary>,
        Decoder<fifi::binary>
        >(symbols, symbol_size);

    run_test_on_the_fly_systematic_no_errors
        <
        Encoder<fifi::binary8>,
        Decoder<fifi::binary8>
        >(symbols, symbol_size);

    run_test_on_the_fly_systematic_no_errors
        <
        Encoder<fifi::binary16>,
        Decoder<fifi::binary16>
        >(symbols, symbol_size);
}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_on_the_fly_systematic_no_errors()
{
    test_on_the_fly_systematic_no_errors<Encoder, Decoder>(32, 1600);
    test_on_the_fly_systematic_no_errors<Encoder, Decoder>(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_on_the_fly_systematic_no_errors<Encoder, Decoder>(
        symbols, symbol_size);
}


/// This test checks that the systematic packets that are received are
/// correctly reported as decoded
template<class Encoder, class Decoder>
inline void run_test_systematic_packets_decode()
{
    uint32_t symbols = 5;
    uint32_t symbol_size = 1400;

    // Common setting
    typename Encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    typename Decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    std::vector<uint8_t> payload(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    auto symbol_sequence = sak::split_storage(
        sak::storage(data_in), symbol_size);

    // Lets start by specifying the first three symbols
    encoder->set_symbol(0, symbol_sequence[0]);
    encoder->set_symbol(1, symbol_sequence[1]);
    encoder->set_symbol(2, symbol_sequence[2]);

    // Make sure the encoder is systematic
    if(kodo::has_systematic_encoder<Encoder>::value)
        kodo::set_systematic_on(encoder);

    encoder->encode( &payload[0] );
    decoder->decode( &payload[0] );

    EXPECT_TRUE(decoder->is_partial_complete());
    EXPECT_TRUE(decoder->is_symbol_uncoded(0));

    encoder->encode( &payload[0] );
    // Simulate a packet loss

    encoder->encode( &payload[0] );
    decoder->decode( &payload[0] );

    EXPECT_TRUE(decoder->is_partial_complete());
    EXPECT_TRUE(decoder->is_symbol_uncoded(0));
    EXPECT_TRUE(decoder->is_symbol_uncoded(2));

    // We now have to loop since we are producing coded packets
    // and we might generate linear dependent packets
    uint32_t loop = 0;
    while(decoder->symbols_uncoded() != 3)
    {
        encoder->encode( &payload[0] );
        decoder->decode( &payload[0] );
        ++loop;

        // std::cout << "Loop " << loop << std::endl;
    }

    EXPECT_TRUE(decoder->is_partial_complete());
    EXPECT_TRUE(decoder->is_symbol_uncoded(0));
    EXPECT_TRUE(decoder->is_symbol_uncoded(1));
    EXPECT_TRUE(decoder->is_symbol_uncoded(2));

    encoder->set_symbol(3, symbol_sequence[3]);

    encoder->encode( &payload[0] );
    decoder->decode( &payload[0] );

    EXPECT_TRUE(decoder->is_partial_complete());
    EXPECT_TRUE(decoder->is_symbol_uncoded(3));

    // Nothing should happen now since the encoder contains no
    // new symbols
    encoder->encode( &payload[0] );
    decoder->decode( &payload[0] );

    EXPECT_FALSE(decoder->is_partial_complete());

    encoder->encode( &payload[0] );
    decoder->decode( &payload[0] );

    EXPECT_FALSE(decoder->is_partial_complete());

    encoder->set_symbol(4, symbol_sequence[4]);

    encoder->encode( &payload[0] );
    // Packet loss

    while(decoder->symbols_uncoded() != 5)
    {
        encoder->encode( &payload[0] );
        decoder->decode( &payload[0] );
        ++loop;

        // std::cout << "Loop " << loop << std::endl;
    }


}

template
<
    template <class> class Encoder,
    template <class> class Decoder
>
inline void test_systematic_packets_decode()
{
    run_test_systematic_packets_decode<
        Encoder<fifi::binary>, Decoder<fifi::binary> >();

    run_test_systematic_packets_decode<
        Encoder<fifi::binary8>, Decoder<fifi::binary8> >();

    run_test_systematic_packets_decode<
        Encoder<fifi::binary16>, Decoder<fifi::binary16> >();
}
