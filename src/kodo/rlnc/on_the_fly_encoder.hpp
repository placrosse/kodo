// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "on_the_fly_generator.hpp"
#include "../final_coder_factory_pool.hpp"
#include "../rank_info.hpp"
#include "../payload_encoder.hpp"
#include "../payload_rank_encoder.hpp"
#include "../plain_symbol_id_writer.hpp"
#include "../coefficient_value_access.hpp"
#include "../coefficient_info.hpp"
#include "../default_on_systematic_encoder.hpp"
#include "../deep_storage_layers.hpp"
#include "../encode_symbol_tracker.hpp"
#include "../finite_field_layers.hpp"
#include "../linear_block_encoder.hpp"
#include "../symbol_id_encoder.hpp"
#include "../storage_aware_encoder.hpp"
#include "../zero_symbol_encoder.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Complete stack implementing a full RLNC on-the-fly encoder.
    ///
    /// The on-the-fly encoder has the advantage that symbols can be
    /// specified as they arrive at the encoder. This breaks with a
    /// traditional block code where all the data has to be available
    /// before encoding can start.
    ///
    /// Implementation of on the fly RLNC encoder uses a storage aware
    /// generator and storage aware encoder.  The storage aware
    /// generator makes sure that we do not generate non-zero
    /// coefficients for the missing symbols, the storage aware
    /// encoder provides the generator with information about how many
    /// symbols have been specified.
    template<class Field, class TraceTag = kodo::disable_trace>
    class on_the_fly_encoder : public
        // Payload Codec API
        payload_rank_encoder<
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        on_the_fly_generator<
        // Encoder API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        rank_info<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        on_the_fly_encoder<Field, TraceTag>
        > > > > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<on_the_fly_encoder>;
    };
}
