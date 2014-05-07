// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "full_rlnc_decoder.hpp"
#include "../mutable_shallow_storage_layers.hpp"

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
        basic_proxy_stack<proxy_args<>, full_rlnc_recoding_stack,
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
        mutable_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        shallow_full_rlnc_decoder<Field, TraceTag>
        > > > > > > > > > > >
    { };
}
