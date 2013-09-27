// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_linear_block_decoder_delayed.cpp Unit tests for the
///       has_linear_block_decoder_delayed class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_linear_block_decoder_delayed.hpp>
#include <kodo/rlnc/full_vector_codes.hpp>
#include <kodo/rlnc/on_the_fly_codes.hpp>

namespace kodo
{
        /// RLNC decoder using Gaussian elimination decoder, delayed
    /// here refers to the fact the we will not perform the backwards
    /// substitution until we have reached full rank
    template<class Field>
    class full_delayed_rlnc_decoder
        : public // Payload API
                 payload_recoder<recoding_stack,
                 payload_decoder<
                 // Codec Header API
                 systematic_decoder<
                 symbol_id_decoder<
                 // Symbol ID API
                 plain_symbol_id_reader<
                 // Decoder API
                 aligned_coefficients_decoder<
                 linear_block_decoder_delayed<
                 forward_linear_block_decoder<
                 symbol_decoding_status_counter<
                 symbol_decoding_status_tracker<
                 // Coefficient Storage API
                 coefficient_value_access<
                 coefficient_storage<
                 coefficient_info<
                 // Storage API
                 deep_symbol_storage<
                 storage_bytes_used<
                 storage_block_info<
                 // Finite Field API
                 finite_field_math<typename fifi::default_field<Field>::type,
                 finite_field_info<Field,
                 // Factory API
                 final_coder_factory_pool<
                 // Final type
                 full_delayed_rlnc_decoder<Field>
                     > > > > > > > > > > > > > > > > > > >
    { };

}

TEST(TestHasLinearBlockDecoderDelayed, detect)
{
    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::full_rlnc_encoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::full_rlnc_encoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::full_rlnc_decoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::full_rlnc_decoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::on_the_fly_encoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::on_the_fly_encoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::on_the_fly_decoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::on_the_fly_decoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::full_rlnc_encoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                     kodo::full_rlnc_encoder<fifi::binary8> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                    kodo::debug_full_rlnc_decoder<fifi::binary> >::value);

    EXPECT_FALSE(kodo::has_linear_block_decoder_delayed<
                    kodo::debug_full_rlnc_decoder<fifi::binary8> >::value);

    EXPECT_TRUE(kodo::has_linear_block_decoder_delayed<
                    kodo::full_delayed_rlnc_decoder<fifi::binary> >::value);

    EXPECT_TRUE(kodo::has_linear_block_decoder_delayed<
                    kodo::full_delayed_rlnc_decoder<fifi::binary8> >::value);

}



