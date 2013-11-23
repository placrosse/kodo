// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "on_the_fly_codes.hpp"

#include "../pivot_status_writer.hpp"
#include "../feedback_pivot_status_writer.hpp"
#include "../final_feedback_writer.hpp"

namespace kodo
{

    /// @todo document
    /// @ingroup fec_stacks
    /// @brief Implementation of a complete RLNC decoder
    ///
    /// This configuration adds the following features (including those
    /// described for the encoder):
    /// - Recoding using the recoding_stack
    /// - Linear block decoder using Gauss-Jordan elimination.
    template<class Field>
    class sliding_window_decoder :
        public // Feedback API
               feedback_pivot_status_writer<
               final_feedback_writer<
               // Payload API
               partial_decoding_tracker<
               rank_symbol_decoding_status_updater<
               payload_recoder<on_the_fly_recoding_stack,
               payload_rank_decoder<
               payload_decoder<
               // Codec Header API
               systematic_decoder<
               symbol_id_decoder<
               // Symbol ID API
               plain_symbol_id_reader<
               // Decoder API
               aligned_coefficients_decoder<
               forward_linear_block_decoder<
               pivot_status_writer<
               rank_info<
               symbol_decoding_status_counter<
               symbol_decoding_status_tracker<
               // Coefficient Storage API
               coefficient_value_access<
               coefficient_storage<
               coefficient_info<
               // Storage API
               deep_symbol_storage<
               storage_bytes_used<
               storage_block_info<
               // Finite Field API
               finite_field_math<typename fifi::default_field<Field>::type,
               finite_field_info<Field,
               // Factory API
               final_coder_factory_pool<
               // Final type
               sliding_window_decoder<Field>
                   > > > > > > > > > > > > > > > > > > > > > > > > >
    { };

}



