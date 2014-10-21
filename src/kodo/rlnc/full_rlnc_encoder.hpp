// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "../final_layer.hpp"
#include "../zero_symbol_encoder.hpp"
#include "../default_on_systematic_encoder.hpp"
#include "../payload_encoder.hpp"
#include "../symbol_id_encoder.hpp"
#include "../coefficient_storage.hpp"
#include "../coefficient_info.hpp"
#include "../plain_symbol_id_writer.hpp"
#include "../uniform_generator_layers.hpp"
#include "../storage_aware_encoder.hpp"
#include "../encode_symbol_tracker.hpp"
#include "../linear_block_encoder.hpp"
#include "../coefficient_value_access.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    /// @brief Complete stack implementing a RLNC encoder.
    ///
    /// The key features of this configuration is the following:
    /// - Systematic encoding (uncoded symbols produced before switching
    ///   to coding)
    /// - Full encoding vectors, this stack uses the plain_symbol_id_writer
    ///   which sends the full encoding vector with every encoded symbol.
    ///   Encoding vectors are generated using a random uniform generator.
    /// - Deep symbol storage which makes the encoder allocate its own
    ///   internal memory.
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        uniform_generator_layers<
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder>;
    };
}
