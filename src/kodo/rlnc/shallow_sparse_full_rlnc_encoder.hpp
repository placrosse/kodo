// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../default_on_systematic_encoder.hpp"
#include "../partial_shallow_storage_layers.hpp"
#include "../sparse_uniform_generator.hpp"
#include "../finite_field_layers.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Complete stack implementing a shallow storage sparse RLNC
    ///        decoder.
    ///
    /// The decoder is identical to the full_rlnc_decoder except for
    /// the fact that is uses a shallow storage layer. Furthermore the
    /// RLNC encoder using a density based random generator, which can be
    /// used to control the density i.e. the number of non-zero elements in
    /// the encoding vector.
    template<class Field, class TraceTag = kodo::disable_trace>
    class shallow_sparse_full_rlnc_encoder : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        sparse_uniform_generator<
        // Encoder API
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
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<shallow_sparse_full_rlnc_encoder>;
    };
}
