// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../payload_encoder.hpp"
#include "../final_layer.hpp"
#include "../finite_field_info.hpp"
#include "../disable_trace.hpp"
#include "../deep_storage_layers.hpp"

#include "carousel_encoder.hpp"
#include "nocode_encoder.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief For testing purposes this encoder implements a simple repetition
    ///        code i.e. the symbols are the original source symbols produced
    ///        by the encoder in a round-robin fashion.
    template<class TraceTag = kodo::disable_trace>
    class nocode_carousel_encoder : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        carousel_encoder<
        // Codec API
        nocode_encoder<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_info<fifi::binary,
        // Final Layer
        final_layer
        > > > > >
    {
    public:
        using factory = pool_factory<nocode_carousel_encoder>;
    };
}
