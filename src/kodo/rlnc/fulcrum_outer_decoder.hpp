// Copyright Steinwurf ApS 2011-2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../nested_payload_decoder.hpp"
#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"
#include "../systematic_coefficient_mapper.hpp"
#include "../uniform_generator.hpp"

#include "fulcrum_info.hpp"
#include "fulcrum_payload_decoder.hpp"
#include "fulcrum_nested_proxy_stack.hpp"

#include "fulcrum_outer_symbol_mapper.hpp"

namespace kodo
{
    /// @todo test + docs
    template<class Field, class TraceTag = kodo::disable_trace>
    class fulcrum_outer_decoder : public
        // Payload API
        nested_payload_decoder<
        fulcrum_nested_proxy_stack<fulcrum_payload_decoder,
        // Decoder API
        fulcrum_outer_symbol_mapper<
        systematic_coefficient_mapper<
        // Coefficient Generator API
        uniform_generator<
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
        fulcrum_outer_decoder<Field, TraceTag>
        > > > > > > > > > > >
    { };
}
