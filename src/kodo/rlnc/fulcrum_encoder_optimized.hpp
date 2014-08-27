// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../nested_toggle_systematic.hpp"
#include "../payload_precoder.hpp"
#include "../systematic_precoder.hpp"
#include "../systematic_coefficient_mapper.hpp"
#include "../nested_payload_encoder.hpp"

#include "shallow_full_rlnc_encoder.hpp"
#include "fulcrum_nested_stack.hpp"
#include "fulcrum_info.hpp"

#include <kodo/new_uniform_generator.hpp>

#include <boost/random/mersenne_twister.hpp>           //mt11213b and mt19937
#include <boost/random/shuffle_order.hpp>              //knuth_b and kreutzer1986
#include <boost/random/taus88.hpp>                     //taus88

namespace kodo
{
    /// @todo docs + tests
    ///
    template<class Field, class TraceTag = kodo::disable_trace>
    class fulcrum_encoder_mt19937_uint8 : public
        // Payload Codec API
        nested_toggle_systematic<
        payload_precoder<
        systematic_precoder<
        systematic_coefficient_mapper<
        nested_payload_encoder<
        fulcrum_nested_stack<shallow_full_rlnc_encoder<fifi::binary>,
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt19937,
        uint8_t,
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
        // Fulcrum API
        fulcrum_info<10,4,
        // Factory API
        final_coder_factory_pool<
        // Final type
        fulcrum_encoder<Field, TraceTag>
        > > > > > > > > > > > > > > > > >
    { };
}
