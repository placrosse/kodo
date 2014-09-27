// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "../coefficient_storage_layers.hpp"
#include "../deep_storage_layers.hpp"
#include "../final_layer.hpp"
#include "../finite_field_layers.hpp"
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
    /// @brief Implementation of a complete perpetual decoder
    ///
    /// This configuration adds the following features (including those
    /// described for the encoder):
    /// - Recoding using the recoding_stack
    /// - Linear block decoder using Gauss-Jordan elimination.
    template<class Field, class TraceTag = kodo::disable_trace>
    class perpetual_decoder : public
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
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_layer
        // Final type
        > > > > > > > > >
    {
    public:
        using factory = pool_factory<perpetual_decoder>;
};
}
