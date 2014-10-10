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
#include "../basic_symbol_decoder.hpp"

#include "fulcrum_info.hpp"
#include "fulcrum_payload_decoder.hpp"
#include "fulcrum_proxy_stack.hpp"
#include "fulcrum_two_stage_decoder.hpp"

namespace kodo
{
    /// @todo clean up + tests + docs
    ///
    /// @ingroup fec_stacks
    ///
    /// @brief The fulcrum combined decoder tries to decode as much as
    ///        possible in the inner code before passing symbols to the outer
    ///        code.
    ///
    /// The main component of the combined decoder is implemented in
    /// the fulcrum_two_stage_decoder layer. Which implements decoding
    /// in the inner code, as soon as a full decoding is deem possible
    /// symbols are taken from the fulcrum_two_stage_decoder and pass
    /// to the main decoder (which decodes in the outer code).
    ///
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
            basic_symbol_decoder<fifi::binary, TraceTag>,
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
        // Final Layer
        final_layer
        > > > > > > > > > >
    {
    public:
        using factory = pool_factory<fulcrum_combined_decoder>;
    };
}
