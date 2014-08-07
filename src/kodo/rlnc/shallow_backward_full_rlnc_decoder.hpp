// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../final_layer.hpp"
#include "../systematic_decoder.hpp"
#include "../payload_decoder.hpp"
#include "../symbol_id_decoder.hpp"
#include "../plain_symbol_id_reader.hpp"
#include "../proxy_stack.hpp"
#include "../nested_payload_recoder.hpp"
#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"
#include "../partial_mutable_shallow_storage_layers.hpp"
#include "../finite_field_layers.hpp"
#include "../backward_linear_block_decoder.hpp"

#include "full_rlnc_recoding_stack.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Complete stack implementing a shallow storage backward RLNC
    ///        decoder.
    ///
    /// The decoder is identical to the full_rlnc_decoder except for
    /// the fact that is uses a shallow storage layer and that it
    /// performs gaussian elimination in the opposite direction
    /// (i.e. from right to left).
    template<class Field, class TraceTag = kodo::disable_trace>
    class shallow_backward_full_rlnc_decoder : public
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
        backward_linear_block_decoder<
        symbol_decoding_status_counter<
        symbol_decoding_status_tracker<
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        partial_mutable_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<shallow_backward_full_rlnc_decoder>;
    };
}
