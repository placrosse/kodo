// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <gtest/gtest.h>

#include "basic_api_test_helper.hpp"

#include <kodo/has_set_systematic_off.hpp>
#include <kodo/set_systematic_off.hpp>

#include <kodo/write_feedback.hpp>
#include <kodo/read_feedback.hpp>
#include <kodo/feedback_size.hpp>

#include <kodo/has_shallow_symbol_storage.hpp>
#include <kodo/has_deep_symbol_storage.hpp>

/// Helper for the reuse test, ensures that all encoders and decoders
/// produce valid data
template<class EncoderPointer, class DecoderPointer>
inline void test_reuse_helper(EncoderPointer encoder, DecoderPointer decoder)
{
    std::vector<uint8_t> payload(encoder->payload_size());

    std::vector<uint8_t> data_in = random_vector(encoder->block_size());
    std::vector<uint8_t> data_out(decoder->block_size(), '\0');

    sak::mutable_storage storage_in = sak::storage(data_in);

    encoder->set_symbols(storage_in);

    // We need the actual data type not the shared_ptr
    typedef typename EncoderPointer::element_type encoder_type;
    typedef typename DecoderPointer::element_type decoder_type;

    uint32_t feedback_size = 0;

    if (kodo::has_write_feedback<decoder_type>::value)
    {
        EXPECT_EQ(kodo::feedback_size(encoder),
                  kodo::feedback_size(decoder));

        feedback_size = kodo::feedback_size(encoder);
        EXPECT_TRUE(feedback_size > 0);
    }

    std::vector<uint8_t> feedback(feedback_size);


    // If the decoder uses shallow storage we have to initialize
    // it's decoding buffers
    if (kodo::has_shallow_symbol_storage<decoder_type>::value)
    {
        decoder->set_symbols(sak::storage(data_out));
    }

    // Set the encoder non-systematic
    if (kodo::has_set_systematic_off<encoder_type>::value)
    {
        kodo::set_systematic_off(encoder);
    }

    while (!decoder->is_complete())
    {
        uint32_t payload_used = encoder->encode( &payload[0] );
        EXPECT_TRUE(payload_used <= encoder->payload_size());

        decoder->decode( &payload[0] );

        if (kodo::has_write_feedback<decoder_type>::value)
        {
            uint32_t written = kodo::write_feedback(decoder, &feedback[0]);
            EXPECT_TRUE(written > 0);

            // Pass to the encoder
            kodo::read_feedback(encoder, &feedback[0]);
        }
    }

    // If the decoder uses deep storage we need to copy out the
    // decoded data
    if (kodo::has_deep_symbol_storage<decoder_type>::value)
    {
        decoder->copy_symbols(sak::storage(data_out));
    }

    EXPECT_TRUE(data_in == data_out);

}

/// Test that instantiates a number of encoders and decoders from
/// the same factories
template<class Encoder, class Decoder>
inline void run_test_reuse(uint32_t symbols, uint32_t symbol_size)
{
    // Common setting
    kodo::basic_factory<Encoder> encoder_factory(symbols, symbol_size);
    kodo::basic_factory<Decoder> decoder_factory(symbols, symbol_size);

    for (uint32_t i = 0; i < 3; ++i)
    {
        uint32_t coders = rand_nonzero(5);

        std::vector<typename kodo::basic_factory<Encoder>::pointer> encoders;
        std::vector<typename kodo::basic_factory<Decoder>::pointer> decoders;

        for (uint32_t j = 0; j < coders; ++j)
        {
            encoders.push_back(encoder_factory.build());
            decoders.push_back(decoder_factory.build());

        }

        for (uint32_t j = 0; j < coders; ++j)
        {
            test_reuse_helper(encoders[j], decoders[j]);
        }

    }

    // Test with differing symbols and symbol sizes
    for (uint32_t i = 0; i < 2; ++i)
    {
        uint32_t coders = rand_nonzero(3);

        std::vector<typename kodo::basic_factory<Encoder>::pointer> encoders;
        std::vector<typename kodo::basic_factory<Decoder>::pointer> decoders;

        for (uint32_t j = 0; j < coders; ++j)
        {
            uint32_t s = rand_symbols(encoder_factory.max_symbols());
            uint32_t l = rand_symbol_size(encoder_factory.max_symbol_size());

            encoder_factory.set_symbols(s);
            encoder_factory.set_symbol_size(l);
            decoder_factory.set_symbols(s);
            decoder_factory.set_symbol_size(l);

            encoders.push_back(encoder_factory.build());
            decoders.push_back(decoder_factory.build());
        }

        for (uint32_t j = 0; j < coders; ++j)
        {
            test_reuse_helper(encoders[j], decoders[j]);
        }

    }

}

/// Test reuse function which will invoke the reuse test for the common
/// field sizes
template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_reuse(uint32_t symbols, uint32_t symbol_size)
{
    run_test_reuse
        <
            Encoder<fifi::binary>,
            Decoder<fifi::binary>
        >(symbols, symbol_size);

    run_test_reuse
        <
            Encoder<fifi::binary8>,
            Decoder<fifi::binary8>
        >(symbols, symbol_size);

    run_test_reuse
        <
            Encoder<fifi::binary16>,
            Decoder<fifi::binary16>
        >(symbols, symbol_size);
}

template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_reuse()
{
    test_reuse<Encoder, Decoder>(1, 1600);
    test_reuse<Encoder, Decoder>(32, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_reuse<Encoder, Decoder>(symbols, symbol_size);
}



/// Tests that encoders and decoders can be safely reused after incomplete
/// encoding / decoding.
template<class Encoder, class Decoder>
inline void run_test_reuse_incomplete(uint32_t symbols, uint32_t symbol_size)
{

    bool do_complete;

    kodo::basic_factory<Encoder> encoder_factory(symbols, symbol_size);
    kodo::basic_factory<Decoder> decoder_factory(symbols, symbol_size);

    // Use factory a lot of times
    for (uint32_t i = 0; i < 20; ++i)
    {
        // Build coders
        auto encoder = encoder_factory.build();
        auto decoder = decoder_factory.build();

        // Prepare buffers
        std::vector<uint8_t> payload(encoder->payload_size());
        std::vector<uint8_t> data_in(encoder->block_size());
        std::vector<uint8_t> data_out(decoder->block_size());

        // Fill with random data
        std::generate(data_in.begin(), data_in.end(), rand);

        // Put data in encoder
        encoder->set_symbols(sak::storage(data_in));

        // If the decoder uses shallow storage we have to initialize
        // it's decoding buffers
        if (kodo::has_shallow_symbol_storage<Decoder>::value)
        {
            decoder->set_symbols(sak::storage(data_out));
        }

        if (rand() % 100 > 90)
        {
            do_complete = false;
        }
        else
        {
            do_complete = true;
        }

        // Start encoding/decoding
        while (!decoder->is_complete())
        {
            encoder->encode(&payload[0]);

            // Loose a packet with probability
            if (rand() % 100 > 90)
            {
                continue;
            }

            decoder->decode(&payload[0]);

            // Stop decoding after a while with probability
            assert(symbols > 2);
            if (!do_complete && decoder->rank() == (symbols - 2))
            {
                break;
            }

        }

        // Check if completed decoders are correct
        if (decoder->is_complete())
        {
            // If the decoder uses deep storage we need to copy out the
            // decoded data
            if (kodo::has_deep_symbol_storage<Decoder>::value)
            {
                decoder->copy_symbols(sak::storage(data_out));
            }

            EXPECT_TRUE(data_in == data_out);
        }
    }

}


/// Test reuse function which will invoke the reuse test for the common
/// field sizes
template
    <
    template <class...> class Encoder,
    template <class...> class Decoder
    >
inline void test_reuse_incomplete(uint32_t symbols, uint32_t symbol_size)
{
    run_test_reuse_incomplete
        <
            Encoder<fifi::binary>,
            Decoder<fifi::binary>
        >(symbols, symbol_size);

    run_test_reuse_incomplete
        <
            Encoder<fifi::binary8>,
            Decoder<fifi::binary8>
        >(symbols, symbol_size);

    run_test_reuse_incomplete
        <
            Encoder<fifi::binary16>,
            Decoder<fifi::binary16>
        >(symbols, symbol_size);
}


template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_reuse_incomplete()
{
    test_reuse_incomplete<Encoder, Decoder>(4, 1600);
    test_reuse_incomplete<Encoder, Decoder>(32, 1600);

    // The test requires that we have at least two symbols
    uint32_t symbols = rand_symbols() + 2;
    uint32_t symbol_size = rand_symbol_size();

    test_reuse_incomplete<Encoder, Decoder>(symbols, symbol_size);
}
