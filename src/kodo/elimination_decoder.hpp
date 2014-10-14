// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <fifi/default_field.hpp>

#include "payload_decoder.hpp"
#include "symbol_id_decoder.hpp"
#include "plain_symbol_id_reader.hpp"
#include "aligned_coefficients_decoder.hpp"
#include "forward_linear_block_decoder.hpp"
#include "symbol_decoding_status_tracker.hpp"
#include "symbol_decoding_status_counter.hpp"
#include "coefficient_storage.hpp"
#include "deep_symbol_storage.hpp"
#include "storage_bytes_used.hpp"
#include "storage_block_size.hpp"
#include "storage_block_length.hpp"
#include "final_layer.hpp"
#include "finite_field_math.hpp"
#include "finite_field_info.hpp"
#include "coefficient_info.hpp"
#include "elimination_coefficient_info.hpp"
#include "elimination_coefficient_value_access.hpp"
#include "elimination_coefficient_offset.hpp"
#include "deep_storage_layers.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief The elimination decoder uses a non-square decoding
    ///        matrix to ignore certain coding coding
    ///        coefficients. One typical use for this is create an
    ///        elimination decoer for a specific part of encoded
    ///        symbols when the elimination decoder reaches full rank
    ///        it will effectively be able to eliminate that part for
    ///        the encoding, while still producing valid encoded
    ///        symbols.
    template<class Field, class TraceTag = kodo::disable_trace>
    class elimination_decoder : public
        // Decoder API
        forward_linear_block_decoder<
        symbol_decoding_status_counter<
        symbol_decoding_status_tracker<
        // Coefficient Storage API
        coefficient_storage<
        elimination_coefficient_value_access<
        elimination_coefficient_info<
        elimination_coefficient_offset<
        coefficient_value_access<
        coefficient_info<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<elimination_decoder>;
    };
}
