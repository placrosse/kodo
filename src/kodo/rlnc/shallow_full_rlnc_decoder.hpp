// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../default_on_systematic_encoder.hpp"
#include "../has_shallow_symbol_storage.hpp"
#include "../linear_block_decoder_delayed.hpp"
#include "../partial_mutable_shallow_storage_layers.hpp"

#include "../common_decoder_layers.hpp"
#include "../finite_field_layers.hpp"
#include "../coefficient_storage_layers.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Complete stack implementing a shallow storage RLNC decoder.
    ///
    /// The decoder is identical to the full_rlnc_decoder except for
    /// the fact that is uses a shallow storage layer.
    template<class Field, class TraceTag = kodo::disable_trace>
    class shallow_full_rlnc_decoder : public
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
        common_decoder_layers<TraceTag,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        partial_mutable_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > >
    {
    public:
        using factory = pool_factory<shallow_full_rlnc_decoder>;
    };
}
