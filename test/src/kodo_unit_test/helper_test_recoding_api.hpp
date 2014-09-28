// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

/// @file helper_recoding_api.hpp Unit test helper for testing that recoding
///       works

#include <ctime>
#include <gtest/gtest.h>

#include <kodo/has_shallow_symbol_storage.hpp>
#include <kodo/has_systematic_encoder.hpp>
#include <kodo/set_systematic_off.hpp>

#include <kodo/has_trace.hpp>
#include <kodo/trace.hpp>

/// Small helper structure holding the parameters needed for the
/// recoding tests.
struct recoding_parameters
{
    uint32_t m_max_symbols;
    uint32_t m_max_symbol_size;
    uint32_t m_symbols;
    uint32_t m_symbol_size;

    bool m_systematic_encoder;
};

// Output operator for the recoding parameters
// @param out The output stream (ostream) to write to
// @param p The recoding_parameter struct to write
// @return The output stream
inline std::ostream& operator<<(std::ostream& out,
                                const recoding_parameters& p)
{
    out << "Recoding parameters:" << std::endl;
    out << "  m_max_symbols = " << p.m_max_symbols << std::endl;
    out << "  m_max_symbol_size = " << p.m_max_symbol_size << std::endl;
    out << "  m_symbols = " << p.m_symbols << std::endl;
    out << "  m_symbol_size = " << p.m_symbol_size << std::endl;
    out << "  m_systematic_encoder = " << p.m_systematic_encoder << std::endl;
    return out;
}

/// Tests that the recoding function works, this is done by using one
/// encoder and two decoders sequentially:
///
///    +------------+      +------------+      +------------+
///    | encoder    |+---->| decoder #1 |+---->| decoder #2 |
///    +------------+      +------------+      +------------+
///
/// Where the encoder passes data to the first decoder which then
/// recodes and passes data to the second decoder
///
/// @param param The recoding parameters to use
template<class Encoder, class Decoder>
inline void invoke_recoding(recoding_parameters param)
{
    // Common setting
    kodo::basic_factory<Encoder> encoder_factory(
        param.m_max_symbols, param.m_max_symbol_size);

    encoder_factory.set_symbols(param.m_symbols);
    encoder_factory.set_symbol_size(param.m_symbol_size);

    auto encoder = encoder_factory.build();

    kodo::basic_factory<Decoder> decoder_factory(
        param.m_max_symbols, param.m_max_symbol_size);

    decoder_factory.set_symbols(param.m_symbols);
    decoder_factory.set_symbol_size(param.m_symbol_size);

    auto decoder_one = decoder_factory.build();
    auto decoder_two = decoder_factory.build();

    // If tested with a shallow decoder we have to remember to set the
    // buffers to use for the decoding
    std::vector<uint8_t> buffer_decoder_one(decoder_one->block_size(), '\0');
    std::vector<uint8_t> buffer_decoder_two(decoder_two->block_size(), '\0');

    // If the decoder uses shallow storage we have to initialize
    // it's decoding buffers
    if (kodo::has_shallow_symbol_storage<Decoder>::value)
    {
        decoder_one->set_symbols(sak::storage(buffer_decoder_one));
        decoder_two->set_symbols(sak::storage(buffer_decoder_two));
    }

    // In case our encoder/decoders support feedback
    std::vector<uint8_t> feedback;

    if (kodo::has_feedback_size<Encoder>::value)
    {
        // If encoder has the feedback size so should the decoder
        ASSERT_TRUE(kodo::has_feedback_size<Decoder>::value);

        EXPECT_EQ(kodo::feedback_size(encoder),
                  kodo::feedback_size(decoder_one));

        EXPECT_EQ(kodo::feedback_size(encoder),
                  kodo::feedback_size(decoder_two));

        feedback.resize(kodo::feedback_size(encoder));
    }

    EXPECT_EQ(encoder->payload_size(), decoder_one->payload_size());
    EXPECT_EQ(encoder->payload_size(), decoder_two->payload_size());

    std::vector<uint8_t> payload(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    encoder->set_symbols(sak::storage(data_in));

    // Set the encoder non-systematic
    if (kodo::has_systematic_encoder<Encoder>::value)
    {
        kodo::set_systematic_off(encoder);
    }

    while (!decoder_two->is_complete())
    {
        uint32_t encode_size = encoder->encode( &payload[0] );
        EXPECT_TRUE(encode_size <= payload.size());
        EXPECT_TRUE(encode_size > 0);

        decoder_one->decode(payload.data());

        if (kodo::has_trace<Decoder>::value)
        {
            std::cout << "Decoder one" << std::endl;
            kodo::trace(decoder_one, std::cout);
        }

        // Pass feedback if available
        if (kodo::has_write_feedback<Decoder>::value)
        {
            uint32_t size = kodo::write_feedback(decoder_one, feedback.data());

            EXPECT_TRUE(size > 0);
            EXPECT_TRUE(size <= feedback.size());

            kodo::read_feedback(encoder, feedback.data());
        }

        uint32_t recode_size = decoder_one->recode(payload.data());
        EXPECT_TRUE(recode_size <= payload.size());
        EXPECT_TRUE(recode_size > 0);

        decoder_two->decode(payload.data());

        if (kodo::has_trace<Decoder>::value)
        {
            std::cout << "Decoder two" << std::endl;
            kodo::trace(decoder_two, std::cout);
        }

        // Pass feedback if available
        if (kodo::has_write_feedback<Decoder>::value)
        {
            uint32_t size = kodo::write_feedback(decoder_two, feedback.data());

            EXPECT_TRUE(size > 0);
            EXPECT_TRUE(size <= feedback.size());

            kodo::read_feedback(decoder_one, feedback.data());
        }
    }

    std::vector<uint8_t> data_out_one(decoder_one->block_size(), '\0');
    std::vector<uint8_t> data_out_two(decoder_two->block_size(), '\0');

    decoder_one->copy_symbols(sak::storage(data_out_one));
    decoder_two->copy_symbols(sak::storage(data_out_two));

    EXPECT_TRUE(data_out_one == data_in);
    EXPECT_TRUE(data_out_two == data_in);
}

/// Invokes the recoding API for the Encoder and Decoder with
/// typical field sizes
/// @param param The recoding parameters
template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_recoders(recoding_parameters param)
{
    SCOPED_TRACE(testing::Message() << param);

    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        invoke_recoding<
            Encoder<fifi::binary>,
            Decoder<fifi::binary> >(param);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        invoke_recoding<
            Encoder<fifi::binary8>,
            Decoder<fifi::binary8> >(param);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        invoke_recoding<
            Encoder<fifi::binary16>,
            Decoder<fifi::binary16> >(param);
    }
}

/// Invokes the recoding API for the Encoder and Decoder with
/// typical field sizes
/// @param param The recoding parameters
template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_recoders()
{
    recoding_parameters param;
    param.m_max_symbols = 32;
    param.m_max_symbol_size = 1600;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = false;

    test_recoders<Encoder,Decoder>(param);

    param.m_max_symbols = 1;
    param.m_max_symbol_size = 1600;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = false;

    test_recoders<Encoder,Decoder>(param);

    param.m_max_symbols = 8;
    param.m_max_symbol_size = 8;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = false;

    test_recoders<Encoder,Decoder>(param);

    param.m_max_symbols = rand_symbols();
    param.m_max_symbol_size = rand_symbol_size();
    param.m_symbols = rand_symbols(param.m_max_symbols);
    param.m_symbol_size = rand_symbol_size(param.m_max_symbol_size);
    param.m_systematic_encoder = false;

    test_recoders<Encoder,Decoder>(param);
}

/// Tests that the recoding function works, this is done by using one
/// encoder and two decoders:
///
///          +---------------------------------------+
///          |                                       |
///          +                                       v
///    +------------+      +------------+      +------------+
///    | encoder    |+---->| decoder #1 |+---->| decoder #2 |
///    +------------+      +------------+      +------------+
///
/// Where the encoder passes data to the first decoder and second
/// decoder. The first decoder then recodes and passes data to the
/// second decoder.
///
/// @param param The recoding parameters to use
template<class Encoder, class Decoder>
inline void run_test_recoding_relay(recoding_parameters param)
{
    // Common setting
    kodo::basic_factory<Encoder> encoder_factory(
        param.m_max_symbols, param.m_max_symbol_size);

    encoder_factory.set_symbols(param.m_symbols);
    encoder_factory.set_symbol_size(param.m_symbol_size);

    auto encoder = encoder_factory.build();

    kodo::basic_factory<Decoder> decoder_factory(
        param.m_max_symbols, param.m_max_symbol_size);

    decoder_factory.set_symbols(param.m_symbols);
    decoder_factory.set_symbol_size(param.m_symbol_size);

    auto decoder_one = decoder_factory.build();
    auto decoder_two = decoder_factory.build();

    // If tested with a shallow decoder we have to remember to set the
    // buffers to use for the decoding
    std::vector<uint8_t> buffer_decoder_one(decoder_one->block_size(), '\0');
    std::vector<uint8_t> buffer_decoder_two(decoder_two->block_size(), '\0');

    if(kodo::has_shallow_symbol_storage<Decoder>::value)
    {
        decoder_one->set_symbols(sak::storage(buffer_decoder_one));
        decoder_two->set_symbols(sak::storage(buffer_decoder_two));
    }

    EXPECT_EQ(encoder->payload_size(), decoder_one->payload_size());
    EXPECT_EQ(encoder->payload_size(), decoder_two->payload_size());

    std::vector<uint8_t> payload_one(encoder->payload_size());
    std::vector<uint8_t> payload_two(encoder->payload_size());
    std::vector<uint8_t> data_in = random_vector(encoder->block_size());

    encoder->set_symbols(sak::storage(data_in));

    // Set the encoder non-systematic
    if(kodo::has_systematic_encoder<Encoder>::value &&
       !param.m_systematic_encoder)
    {
        kodo::set_systematic_off(encoder);
    }

    while( !(decoder_two->is_complete() && decoder_one->is_complete()) )
    {
        uint32_t encode_size = encoder->encode(payload_one.data());
        EXPECT_TRUE(encode_size <= payload_one.size());
        EXPECT_TRUE(encode_size > 0);

        // Copy the encoded symbol into payload two
        payload_two = payload_one;
        decoder_two->decode(payload_two.data());

        if((rand() % 2) == 0)
        {
            decoder_one->decode(payload_one.data());

            uint32_t recode_size = decoder_one->recode(payload_one.data());

            EXPECT_TRUE(recode_size <= payload_one.size());
            EXPECT_TRUE(recode_size > 0);

            decoder_two->decode(payload_one.data());
        }

    }

    std::vector<uint8_t> data_out_one(decoder_one->block_size(), '\0');
    std::vector<uint8_t> data_out_two(decoder_two->block_size(), '\0');

    decoder_one->copy_symbols(sak::storage(data_out_one));
    decoder_two->copy_symbols(sak::storage(data_out_two));

    EXPECT_TRUE(data_out_one == data_in);
    EXPECT_TRUE(data_out_two == data_in);
}

/// Invokes the recoding API for the Encoder and Decoder with
/// typical field sizes
/// @param param The recoding parameters
template
<
    template <class...> class Encoder,
    template <class...> class Decoder
>
inline void test_recoding_relay(recoding_parameters param)
{
    SCOPED_TRACE(testing::Message() << param);

    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_recoding_relay<
            Encoder<fifi::binary>,
            Decoder<fifi::binary> >(param);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_recoding_relay<
            Encoder<fifi::binary4>,
            Decoder<fifi::binary4> >(param);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_recoding_relay<
            Encoder<fifi::binary8>,
            Decoder<fifi::binary8> >(param);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_recoding_relay<
            Encoder<fifi::binary16>,
            Decoder<fifi::binary16> >(param);
    }
}

/// Invokes the recoding API for the Encoder and Decoder with
/// typical field sizes
/// @param param The recoding parameters
template
<
    template <class...> class Encoder,
    template <class...> class Decoder
    >
inline void test_recoding_relay()
{
    recoding_parameters param;
    param.m_max_symbols = 32;
    param.m_max_symbol_size = 1600;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = false;

    test_recoding_relay<Encoder,Decoder>(param);

    param.m_max_symbols = 1;
    param.m_max_symbol_size = 1600;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = false;

    test_recoding_relay<Encoder,Decoder>(param);

    param.m_max_symbols = 8;
    param.m_max_symbol_size = 8;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = false;

    test_recoding_relay<Encoder,Decoder>(param);

    param.m_max_symbols = rand_symbols();
    param.m_max_symbol_size = rand_symbol_size();
    param.m_symbols = rand_symbols(param.m_max_symbols);
    param.m_symbol_size = rand_symbol_size(param.m_max_symbol_size);
    param.m_systematic_encoder = false;

    test_recoding_relay<Encoder,Decoder>(param);

    param.m_max_symbols = 32;
    param.m_max_symbol_size = 1600;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = true;

    test_recoding_relay<Encoder,Decoder>(param);

    param.m_max_symbols = 1;
    param.m_max_symbol_size = 1600;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = true;

    test_recoding_relay<Encoder,Decoder>(param);

    param.m_max_symbols = 8;
    param.m_max_symbol_size = 8;
    param.m_symbols = param.m_max_symbols;
    param.m_symbol_size = param.m_max_symbol_size;
    param.m_systematic_encoder = true;

    test_recoding_relay<Encoder,Decoder>(param);

    param.m_max_symbols = rand_symbols();
    param.m_max_symbol_size = rand_symbol_size();
    param.m_symbols = rand_symbols(param.m_max_symbols);
    param.m_symbol_size = rand_symbol_size(param.m_max_symbol_size);
    param.m_systematic_encoder = true;

    test_recoding_relay<Encoder,Decoder>(param);
}
