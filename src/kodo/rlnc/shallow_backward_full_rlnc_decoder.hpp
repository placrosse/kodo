// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../backward_linear_block_decoder.hpp"
#include "../default_on_systematic_encoder.hpp"
#include "../has_shallow_symbol_storage.hpp"
#include "../linear_block_decoder_delayed.hpp"
#include "../partial_shallow_symbol_storage.hpp"
#include "full_rlnc_codes.hpp"
#include "../shallow_symbol_storage.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    /// @brief Complete stack implementing a shallow storage backward RLNC
    ///        decoder.
    ///
    /// The decoder is identical to the full_rlnc_decoder except for
    /// the fact that is uses a shallow storage layer and that it
    /// performs gaussian elimination in the opposite direction
    /// (i.e. from right to left).
    template<class Field>
    class shallow_backward_full_rlnc_decoder : public
        // Payload API
        payload_recoder<full_rlnc_recoding_stack,
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Decoder API
        aligned_coefficients_decoder<
        backward_linear_block_decoder<
        symbol_decoding_status_counter<
        symbol_decoding_status_tracker<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_storage<
        coefficient_info<
        // Storage API
        mutable_shallow_symbol_storage<
        storage_bytes_used<
        storage_block_info<
        // Finite Field API
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        shallow_backward_full_rlnc_decoder<Field>
        > > > > > > > > > > > > > > > > > >
    { };
}
