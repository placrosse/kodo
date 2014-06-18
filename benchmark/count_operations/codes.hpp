// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/linear_block_decoder_delayed.hpp>
#include <kodo/partial_shallow_symbol_storage.hpp>
#include <kodo/finite_field_counter.hpp>

namespace kodo
{
    template<class Field>
    class full_rlnc_encoder_count : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        uniform_generator<
        // Encoder API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        partial_shallow_symbol_storage<
        storage_bytes_used<
        storage_block_info<
        // Finite Field API
        finite_field_counter<
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        full_rlnc_encoder_count<Field>
        > > > > > > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_count>;
    };

    template<class Field>
    class full_rlnc_decoder_count : public
        // Payload API
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Decoder API
        aligned_coefficients_decoder<
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
        finite_field_counter<
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        full_rlnc_decoder_count<Field>
        > > > > > > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_decoder_count>;
    };

    template<class Field>
    class full_delayed_rlnc_decoder_count : public
        // Payload API
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
        finite_field_counter<
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        full_delayed_rlnc_decoder_count<Field>
        > > > > > > > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_delayed_rlnc_decoder_count>;
    };
}
