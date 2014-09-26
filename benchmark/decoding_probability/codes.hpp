// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once


#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/linear_block_decoder_delayed.hpp>
#include <kodo/sparse_uniform_generator.hpp>

namespace kodo
{

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_delayed_rlnc_decoder : public
        // Payload API
        nested_payload_recoder<
        proxy_stack<proxy_args<>, full_rlnc_recoding_stack,
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
        coefficient_storage_layers<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_delayed_rlnc_decoder>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_decoder_unsystematic : public
        // Payload API
        nested_payload_recoder<
        proxy_stack<proxy_args<>, full_rlnc_recoding_stack,
        payload_decoder<
        // Codec Header API
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
        coefficient_storage_layers<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_decoder_unsystematic>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_unsystematic : public
       // Payload Codec API
        payload_encoder<
        // Codec Header API
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
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_unsystematic>;
    };

    /// RLNC encoder using a density based random generator, which can be
    /// used to control the density i.e. the number of non-zero elements in
    /// the encoding vector.
    template<class Field, class TraceTag = kodo::disable_trace>
    class sparse_full_rlnc_encoder : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        sparse_uniform_generator<
        // Encoder API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<sparse_full_rlnc_encoder>;
    };
}
