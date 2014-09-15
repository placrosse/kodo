// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_storage_xyz.cpp Unit tests for storage encoder and decoders

#include <ctime>

#include <gtest/gtest.h>


#include <kodo/rfc5052_partitioning_scheme.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/partial_shallow_symbol_storage.hpp>
#include <kodo/set_systematic_off.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"

// template
// <
//    class Encoder,
//    class Decoder,
//    class Partitioning
// >
// void invoke_shallow_decoder(uint32_t max_symbols,
//                             uint32_t max_symbol_size,
//                             uint32_t object_size)
// {

//     typedef kodo::storage_encoder<Encoder, Partitioning>
//         storage_encoder;

//     typedef kodo::shallow_storage_decoder<Decoder, Partitioning>
//         storage_decoder;

//     typename storage_encoder::factory encoder_factory(
//         max_symbols, max_symbol_size);

//     typename storage_decoder::factory decoder_factory(
//         max_symbols, max_symbol_size);

//     // The storage needed for all decoders
//     uint32_t total_block_size =
//         decoder_factory.total_block_size(object_size);

//     std::vector<uint8_t> data_out(total_block_size, '\0');
//     std::vector<uint8_t> data_in = random_vector(object_size);

//     EXPECT_TRUE(object_size <= total_block_size);

//     storage_encoder encoder(
//         encoder_factory, sak::storage(data_in));

//     storage_decoder decoder(
//         decoder_factory, object_size, sak::storage(data_out));

//     EXPECT_TRUE(encoder.encoders() == decoder.decoders());

//     for(uint32_t i = 0; i < encoder.encoders(); ++i)
//     {
//         auto e = encoder.build(i);
//         auto d = decoder.build(i);

//         if(kodo::has_systematic_encoder<Encoder>::value)
//             kodo::set_systematic_off(e);

//         EXPECT_EQ(e->block_size(), d->block_size());
//         EXPECT_EQ(e->bytes_used(), d->bytes_used());
//         EXPECT_EQ(e->payload_size(), d->payload_size());

//         std::vector<uint8_t> payload(e->payload_size());

//         while(!d->is_complete())
//         {
//             e->encode( &payload[0] );
//             d->decode( &payload[0] );

//         }
//     }

//     // Resize the output buffer to contain only the object data
//     data_out.resize(object_size);

//     EXPECT_TRUE(std::equal(data_in.begin(),
//                            data_in.end(),
//                            data_out.begin()));
// }

// template
// <
//    class Encoder,
//    class Decoder,
//    class Partitioning
// >
// void invoke_deep_decoder(uint32_t max_symbols,
//                          uint32_t max_symbol_size,
//                          uint32_t object_size)
// {

//     typedef kodo::storage_encoder<Encoder, Partitioning>
//         storage_encoder;

//     typedef kodo::deep_storage_decoder<Decoder, Partitioning>
//         storage_decoder;

//     typename storage_encoder::factory encoder_factory(
//         max_symbols, max_symbol_size);

//     typename storage_decoder::factory decoder_factory(
//         max_symbols, max_symbol_size);

//     std::vector<uint8_t> data_out;
//     std::vector<uint8_t> data_in = random_vector(object_size);

//     storage_encoder encoder(
//         encoder_factory, sak::storage(data_in));

//     storage_decoder decoder(
//         decoder_factory, object_size);

//     EXPECT_TRUE(encoder.encoders() == decoder.decoders());

//     for(uint32_t i = 0; i < encoder.encoders(); ++i)
//     {
//         auto e = encoder.build(i);
//         auto d = decoder.build(i);

//         if(kodo::has_systematic_encoder<Encoder>::value)
//             kodo::set_systematic_off(e);

//         EXPECT_EQ(e->block_size(), d->block_size());
//         EXPECT_EQ(e->bytes_used(), d->bytes_used());
//         EXPECT_EQ(e->payload_size(), d->payload_size());

//         std::vector<uint8_t> payload(e->payload_size());

//         while(!d->is_complete())
//         {
//             e->encode( &payload[0] );
//             d->decode( &payload[0] );

//         }
//     }

//     // Get the decoded data buffer
//     decoder.swap(data_out);

//     EXPECT_TRUE(std::equal(data_in.begin(),
//                            data_in.end(),
//                            data_out.begin()));
// }


void test_shallow_decoder(uint32_t symbols,
                          uint32_t symbol_size,
                          uint32_t object_size)
{

    /// @todo re-enable / re-write

    // invoke_shallow_decoder<
    //     kodo::full_rlnc_encoder<fifi::binary>,
    //     kodo::shallow_full_rlnc_decoder<fifi::binary>,
    //     kodo::rfc5052_partitioning_scheme>(
    //         symbols, symbol_size, object_size);

    // invoke_shallow_decoder<
    //     kodo::full_rlnc_encoder<fifi::binary8>,
    //     kodo::shallow_full_rlnc_decoder<fifi::binary8>,
    //     kodo::rfc5052_partitioning_scheme>(
    //         symbols, symbol_size, object_size);

    // invoke_shallow_decoder<
    //     kodo::full_rlnc_encoder<fifi::binary16>,
    //     kodo::shallow_full_rlnc_decoder<fifi::binary16>,
    //     kodo::rfc5052_partitioning_scheme>(
    //         symbols, symbol_size, object_size);

}

TEST(TestStorageCoder, shallow_storage_decoder)
{
    /// @todo re-enable / re-write

    // test_shallow_decoder(32, 1600, 2);
    // test_shallow_decoder(1, 1600, 2);

    // uint32_t symbols = rand_symbols();
    // uint32_t symbol_size = rand_symbol_size();

    // uint32_t object_size = rand_nonzero(symbols * symbol_size * 10);

    // test_shallow_decoder(symbols, symbol_size, object_size);
}


void test_deep_decoder(uint32_t symbols,
                       uint32_t symbol_size,
                       uint32_t object_size)
{
    /// @todo re-enable / re-write

    // invoke_deep_decoder<
    //     kodo::full_rlnc_encoder<fifi::binary>,
    //     kodo::shallow_full_rlnc_decoder<fifi::binary>,
    //     kodo::rfc5052_partitioning_scheme>(
    //         symbols, symbol_size, object_size);

    // invoke_deep_decoder<
    //     kodo::full_rlnc_encoder<fifi::binary8>,
    //     kodo::shallow_full_rlnc_decoder<fifi::binary8>,
    //     kodo::rfc5052_partitioning_scheme>(
    //         symbols, symbol_size, object_size);

    // invoke_deep_decoder<
    //     kodo::full_rlnc_encoder<fifi::binary16>,
    //     kodo::shallow_full_rlnc_decoder<fifi::binary16>,
    //     kodo::rfc5052_partitioning_scheme>(
    //         symbols, symbol_size, object_size);

}

TEST(TestStorageCoder, deep_storage_decoder)
{
    /// @todo re-enable / re-write

    // test_deep_decoder(32, 1600, 2);
    // test_deep_decoder(1, 1600, 2);

    // uint32_t symbols = rand_symbols();
    // uint32_t symbol_size = rand_symbol_size();

    // uint32_t object_size = rand_nonzero(symbols * symbol_size * 10);

    // test_deep_decoder(symbols, symbol_size, object_size);
}
