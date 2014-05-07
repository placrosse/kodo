// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../partial_decoding_tracker.hpp"
#include "../payload_rank_decoder.hpp"
#include "../rank_symbol_decoding_status_updater.hpp"
#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Implementation of a complete RLNC decoder
    ///
    /// This configuration adds the following features (including those
    /// described for the encoder):
    /// - Recoding using the recoding_stack
    /// - Linear block decoder using Gauss-Jordan elimination.
    template<class Field, class TraceTag = kodo::disable_trace>
    class on_the_fly_decoder : public
        // Payload API
        nested_payload_recoder<
        basic_proxy_stack<proxy_args<>, on_the_fly_recoding_stack,
        partial_decoding_tracker<
        rank_symbol_decoding_status_updater<
        payload_rank_decoder<
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
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        on_the_fly_decoder<Field, TraceTag>
        > > > > > > > > > > > > > >

    { };
}
