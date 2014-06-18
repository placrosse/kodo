// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../final_coder_factory_pool.hpp"
#include "../zero_symbol_encoder.hpp"
#include "../default_on_systematic_encoder.hpp"
#include "../payload_encoder.hpp"
#include "../symbol_id_encoder.hpp"
#include "../coefficient_info.hpp"
#include "../coefficient_value_access.hpp"
#include "../storage_aware_encoder.hpp"
#include "../encode_symbol_tracker.hpp"
#include "../linear_block_encoder.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"

#include "reed_solomon_symbol_id_writer.hpp"
#include "reed_solomon_symbol_id_reader.hpp"
#include "systematic_vandermonde_matrix.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Complete stack implementing a Reed-Solomon encoder.
    ///
    /// The key features of this configuration is the following:
    /// - Systematic encoding (uncoded symbols produced before switching
    ///   to coding)
    /// - Deep symbol storage which makes the encoder allocate its own
    ///   internal memory.
    template<class Field, class TraceTag = kodo::disable_trace>
    class reed_solomon_encoder : public
        // Payload API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        reed_solomon_symbol_id_writer<
        systematic_vandermonde_matrix<
        // Encoder API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        reed_solomon_encoder<Field,TraceTag>
        > > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<reed_solomon_encoder>;
    };
}
