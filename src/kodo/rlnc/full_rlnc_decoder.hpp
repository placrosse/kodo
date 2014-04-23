// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <fifi/default_field.hpp>

#include "../aligned_coefficients_decoder.hpp"
#include "../final_coder_factory_pool.hpp"
#include "../final_coder_factory.hpp"
#include "../finite_field_math.hpp"
#include "../finite_field_info.hpp"
#include "../zero_symbol_encoder.hpp"
#include "../default_on_systematic_encoder.hpp"
#include "../default_off_systematic_encoder.hpp"
#include "../systematic_decoder.hpp"
#include "../storage_bytes_used.hpp"
#include "../storage_block_info.hpp"
#include "../deep_symbol_storage.hpp"
#include "../payload_encoder.hpp"
#include "../payload_recoder.hpp"
#include "../payload_decoder.hpp"
#include "../symbol_id_encoder.hpp"
#include "../symbol_id_decoder.hpp"
#include "../coefficient_storage.hpp"
#include "../coefficient_info.hpp"
#include "../plain_symbol_id_reader.hpp"
#include "../plain_symbol_id_writer.hpp"
#include "../uniform_generator.hpp"
#include "../recoding_symbol_id.hpp"
#include "../proxy_layer.hpp"
#include "../proxy_stack.hpp"
#include "../nested_payload_recoder.hpp"
#include "../storage_aware_encoder.hpp"
#include "../encode_symbol_tracker.hpp"
#include "../cached_symbol_decoder.hpp"
#include "../trace_decode_symbol.hpp"
#include "../trace_linear_block_decoder.hpp"
#include "../rank_info.hpp"
#include "../symbol_decoding_status_tracker.hpp"
#include "../symbol_decoding_status_counter.hpp"
#include "../common_decoder_layers.hpp"

#include "../linear_block_encoder.hpp"
#include "../forward_linear_block_decoder.hpp"
#include "../linear_block_decoder_delayed.hpp"
#include "../coefficient_value_access.hpp"
#include "../pivot_aware_generator.hpp"

#include "full_rlnc_recoding_stack.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Implementation of a complete RLNC decoder
    ///
    /// This configuration adds the following features (including those
    /// described for the encoder):
    /// - Recoding using the recoding_stack
    /// - Linear block decoder using Gauss-Jordan elimination.
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_decoder
        : public // Payload API
                 nested_payload_recoder<
                 proxy_stack<proxy_args<>, full_rlnc_recoding_stack,
                 payload_decoder<
                 // Codec Header API
                 systematic_decoder<
                 symbol_id_decoder<
                 // Symbol ID API
                 plain_symbol_id_reader<
                 // Decoder API
                 common_decoder_layers<TraceTag,
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
                 full_rlnc_decoder<Field, TraceTag>
                 > > > > > > > > > > > > > > > >
    { };

}
