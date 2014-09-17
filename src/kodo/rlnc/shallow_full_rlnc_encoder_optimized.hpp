// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../default_on_systematic_encoder.hpp"
#include "../has_shallow_symbol_storage.hpp"
#include "../linear_block_decoder_delayed.hpp"
#include "../partial_shallow_symbol_storage.hpp"
#include "../shallow_symbol_storage.hpp"
#include "../partial_const_shallow_storage_layers.hpp"
#include "../finite_field_layers.hpp"
#include "../new_uniform_generator.hpp"

#include <boost/random/mersenne_twister.hpp>           //mt11213b and mt19937
#include <boost/random/shuffle_order.hpp>              //knuth_b and kreutzer1986
#include <boost/random/taus88.hpp>                     //taus88

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Complete stack implementing a shallow storage RLNC encoder.
    ///
    /// The encoder is identical to the full_rlnc_encoder except for
    /// the fact that is uses a shallow storage layer.
    template<class Field, class ValueType = uint8_t,
        class TraceTag = kodo::disable_trace >
    class shallow_full_rlnc_encoder_mt19937 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt19937,
        ValueType,
        // Encoder API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        partial_const_shallow_storage_layers<TraceTag,
         // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
        public:
            using factory = pool_factory<shallow_full_rlnc_encoder_mt19937>;
    };
}
