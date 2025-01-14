// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../payload_decoder.hpp"
#include "../systematic_decoder.hpp"
#include "../symbol_id_decoder.hpp"
#include "../seed_symbol_id_reader.hpp"
#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Implementation of a seed based RLNC decoder this configuration.
    ///
    /// Adds the following features (including those described for
    /// the encoder):
    /// - Linear block decoder using Gauss-Jordan elimination.
    template<class Field, class TraceTag = kodo::disable_trace>
    class seed_rlnc_decoder : public
        // Payload API
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        seed_symbol_id_reader<
        // Coefficient Generator API
        uniform_generator<
        // Decoder API
        common_decoder_layers<TraceTag,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field Math API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > >
    {
    public:
        using factory = pool_factory<seed_rlnc_decoder>;
    };
}
