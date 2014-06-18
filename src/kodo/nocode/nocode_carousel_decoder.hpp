// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../payload_decoder.hpp"
#include "../final_coder_factory_pool.hpp"
#include "../finite_field_info.hpp"
#include "../deep_storage_layers.hpp"
#include "../disable_trace.hpp"

#include "carousel_decoder.hpp"
#include "nocode_decoder.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief For testing purposes this decoder will takes symbols
    ///        produced by the carousel encoder and put them in the
    ///        right order until all symbols have been received.
    ///
    /// Note that this is a _nocode_ decoder which means that it does
    /// not perform any actual coding. It simply collects uncoded
    /// packets.
    template<class TraceTag = kodo::disable_trace>
    class nocode_carousel_decoder : public
        // Payload Codec API
        payload_decoder<
        // Codec Header API
        carousel_decoder<
        // Codec API
        nocode_decoder<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_info<fifi::binary,
        // Factory API
        final_coder_factory_pool<
        // Final type
        nocode_carousel_decoder<TraceTag>
        > > > > > >
    {
    public:
        using factory = pool_factory<nocode_carousel_decoder>;
    };
}
