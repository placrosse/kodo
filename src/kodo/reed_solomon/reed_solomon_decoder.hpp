// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../final_coder_factory_pool.hpp"
#include "../systematic_decoder.hpp"
#include "../payload_decoder.hpp"
#include "../symbol_id_decoder.hpp"
#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"

#include "reed_solomon_symbol_id_reader.hpp"
#include "systematic_vandermonde_matrix.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Implementation of a complete RS decoder
    ///
    /// This configuration adds the following features (including those
    /// described for the encoder):
    /// - Linear block decoder using Gauss-Jordan elimination.
    template<class Field, class TraceTag = kodo::disable_trace>
    class reed_solomon_decoder : public
        // Payload API
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        reed_solomon_symbol_id_reader<
        systematic_vandermonde_matrix<
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
        reed_solomon_decoder<Field,TraceTag>
        > > > > > > > > > >
    {
    public:
        using factory = pool_factory<reed_solomon_decoder>;
    };
}
