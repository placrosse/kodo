// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "../coefficient_info.hpp"
#include "../coefficient_storage.hpp"
#include "../coefficient_value_access.hpp"
#include "../encode_symbol_tracker.hpp"
#include "../final_coder_factory_pool.hpp"
#include "../finite_field_layers.hpp"
#include "../linear_block_encoder.hpp"
#include "../partial_shallow_storage_layers.hpp"
#include "../payload_encoder.hpp"
#include "../perpetual_generator.hpp"
#include "../plain_symbol_id_writer.hpp"
#include "../storage_aware_encoder.hpp"
#include "../symbol_id_encoder.hpp"
#include "../zero_symbol_encoder.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    /// @brief Complete stack implementing a shallow perpetual encoder.
    ///
    /// The encoder is identical to the perpetual_encoder except for
    /// the fact that is uses a shallow storage layer.
    template<class Field, class TraceTag = kodo::disable_trace>
    class shallow_perpetual_encoder : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        perpetual_generator<
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        partial_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        shallow_perpetual_encoder<Field, TraceTag>
        > > > > > > > > > > > > >
    { };
}
