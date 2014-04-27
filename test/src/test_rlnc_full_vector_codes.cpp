// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing


/// @file test_rlnc_full_vector_codes.cpp Unit tests for the full
///       vector codes (i.e. Network Coding encoders and decoders).

#include <ctime>

#include <gtest/gtest.h>

#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/recoding_symbol_id.hpp>
#include <kodo/proxy_layer.hpp>
#include <kodo/payload_recoder.hpp>
#include <kodo/partial_shallow_symbol_storage.hpp>
#include <kodo/linear_block_decoder_delayed.hpp>
#include <kodo/shallow_symbol_storage.hpp>
#include <kodo/has_shallow_symbol_storage.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"

#include "kodo_unit_test/helper_test_reuse_api.hpp"
#include "kodo_unit_test/helper_test_recoding_api.hpp"
#include "kodo_unit_test/helper_test_on_the_fly_api.hpp"
#include "kodo_unit_test/helper_test_basic_api.hpp"
#include "kodo_unit_test/helper_test_initialize_api.hpp"
#include "kodo_unit_test/helper_test_systematic_api.hpp"
#include "kodo_unit_test/helper_test_mix_uncoded_api.hpp"

/// @todo rename this file like the sliding window files

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        /// Implementation of RLNC decode using the delayed
        /// backwards substitution layer.
        template<class Field>
        class full_rlnc_decoder_delayed
            : public // Payload API
                     payload_recoder<full_rlnc_recoding_stack,
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
                     // Finite Field Math API
                     finite_field_math<typename fifi::default_field<Field>::type,
                     finite_field_info<Field,
                     // Factory API
                     final_coder_factory_pool<
                     // Final type
                     full_rlnc_decoder_delayed<Field>
                         > > > > > > > > > > > > > > > > > > >
        {};

        /// Implementation of RLNC decode using the delayed
        /// backwards substitution layer.
        template<class Field>
        class full_rlnc_decoder_delayed_shallow
            : public // Payload API
                     payload_recoder<full_rlnc_recoding_stack,
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
                     mutable_shallow_symbol_storage<
                     storage_bytes_used<
                     storage_block_info<
                     // Finite Field Math API
                     finite_field_math<typename fifi::default_field<Field>::type,
                     finite_field_info<Field,
                     // Factory API
                     final_coder_factory_pool<
                     // Final type
                     full_rlnc_decoder_delayed_shallow<Field>
                         > > > > > > > > > > > > > > > > > > >
        {};
    }

}


namespace
{
    template<class Field>
    using decoder = kodo::full_rlnc_decoder<Field, kodo::disable_trace>;

    template<class Field>
    using encoder = kodo::full_rlnc_encoder<Field, kodo::disable_trace>;
}

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestRlncFullVectorCodes, test_basic_api)
{
    test_basic_api<kodo::shallow_full_rlnc_encoder, decoder>();

    test_basic_api<encoder, decoder>();

    // The delayed decoders
    test_basic_api<encoder, kodo::full_rlnc_decoder_delayed>();
}



/// Test that the encoders and decoders initialize() function can be used
/// to reset the state of an encoder and decoder and that they therefore
/// can be safely reused.
TEST(TestRlncFullVectorCodes, test_initialize)
{
    test_initialize<kodo::shallow_full_rlnc_encoder, decoder>();

    test_initialize<encoder, decoder>();

    // The delayed decoders
    test_initialize<encoder, kodo::full_rlnc_decoder_delayed>();

}

/// Tests that an encoder producing systematic packets is handled
/// correctly in the decoder.
TEST(TestRlncFullVectorCodes, test_systematic)
{
    test_systematic<encoder,
        decoder>();

    test_systematic<encoder,
        kodo::full_rlnc_decoder_delayed>();
}

/// Tests whether mixed un-coded and coded packets are correctly handled
/// in the encoder and decoder.
TEST(TestRlncFullVectorCodes, mix_uncoded)
{
    test_mix_uncoded<encoder,
        decoder>();

    test_mix_uncoded<encoder,
        kodo::full_rlnc_decoder_delayed>();
}

/// The recoding
TEST(TestRlncFullVectorCodes, test_recoders_api)
{

    test_recoders<encoder,
        decoder>();

    test_recoders<encoder,
        kodo::full_rlnc_decoder_delayed>();

    test_recoders<encoder,
        kodo::full_rlnc_decoder_delayed_shallow>();

}

/// The recoding
TEST(TestRlncFullVectorCodes, test_recoding_relay)
{

    test_recoding_relay<encoder,
        decoder>();

    test_recoding_relay<encoder,
        kodo::full_rlnc_decoder_delayed>();

    test_recoding_relay<encoder,
        kodo::full_rlnc_decoder_delayed_shallow>();

}

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestRlncFullVectorCodes, test_reuse_api)
{
    test_reuse<
        kodo::shallow_full_rlnc_encoder,
        decoder>();

    test_reuse<
        encoder,
        decoder>();

    // The delayed decoders
    test_reuse<
        encoder,
        kodo::full_rlnc_decoder_delayed>();
}

/// Tests the basic API functionality this mean basic encoding
/// and decoding
TEST(TestRlncFullVectorCodes, test_reuse_incomplete_api)
{
    test_reuse_incomplete<
        kodo::shallow_full_rlnc_encoder,
        decoder>();

    test_reuse_incomplete<
        encoder,
        decoder>();

    // The delayed decoders
    test_reuse_incomplete<
        encoder,
        kodo::full_rlnc_decoder_delayed>();
}
