// Copyright Steinwurf ApS 2011-2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../elimination_decoder.hpp"
#include "../nested_payload_decoder.hpp"
#include "../systematic_coefficient_mapper.hpp"

#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"

#include "fulcrum_info.hpp"
#include "fulcrum_payload_decoder.hpp"
#include "fulcrum_proxy_stack.hpp"
#include "fulcrum_two_stage_decoder.hpp"

namespace kodo
{
    /// @todo These layers actually constitute the last part of a lot
    ///       of our decoders maybe we should consider making an
    ///       aggregation layer for it.
    template<class Field, class TraceTag = kodo::disable_trace>
    class stage_two_decoder : public
        // Decoder API
        common_decoder_layers<TraceTag,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        stage_two_decoder<Field, TraceTag>
        > > > > >
    { };

    /// @todo clean up + tests + docs
    template<class Field, class TraceTag = kodo::disable_trace>
    class fulcrum_combined_decoder : public
        // Payload API
        nested_payload_decoder<
        fulcrum_proxy_stack<proxy_args<>, fulcrum_payload_decoder,
        // Codec Header API
        //  - implemented in outer decoder
        // Symbol ID API
        //  - implemented in outer decoder
        // Coefficient Generator API
        fulcrum_two_stage_decoder<
        elimination_decoder<fifi::binary>,
        stage_two_decoder<fifi::binary, TraceTag>,
        systematic_coefficient_mapper<
        uniform_generator<
        // Decoder API
        common_decoder_layers<TraceTag,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Fulcrum API
        fulcrum_info<10,4,
        // Factory API
        final_coder_factory_pool<
        // Final type
        fulcrum_combined_decoder<Field, TraceTag>
        > > > > > > > > > > >
    { };

}
