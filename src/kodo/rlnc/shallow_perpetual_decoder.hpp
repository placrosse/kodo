// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "../coefficient_storage_layers.hpp"
#include "../final_layer.hpp"
#include "../finite_field_layers.hpp"
#include "../partial_mutable_shallow_storage_layers.hpp"
#include "../nested_payload_recoder.hpp"
#include "../payload_decoder.hpp"
#include "../perpetual_decoder_layers.hpp"
#include "../plain_symbol_id_reader.hpp"
#include "../proxy_stack.hpp"
#include "../symbol_id_decoder.hpp"

#include "perpetual_recoding_stack.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Implementation of a shallow perpetual decoder
    ///
    /// The decoder is identical to the perpetual_decoder except for
    /// the fact that is uses a shallow storage layer.
    template<class Field, class TraceTag = kodo::disable_trace>
    class shallow_perpetual_decoder : public
        // Payload API
        nested_payload_recoder<
        proxy_stack<proxy_args<>, perpetual_recoding_stack,
        payload_decoder<
        // Codec Header API
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Decoder API
        perpetual_decoder_layers<TraceTag,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        partial_mutable_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_layer<
        // Final type
        shallow_perpetual_decoder<Field, TraceTag>
        > > > > > > > > > >
    {
    public:
        using factory = pool_factory<shallow_perpetual_decoder>;

};
}
