// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "on_the_fly_codes.hpp"
#include "sliding_window_recoding_stack.hpp"

#include "../pivot_status_writer.hpp"
#include "../feedback_pivot_status_writer.hpp"
#include "../final_feedback_writer.hpp"
#include "../proxy_stack.hpp"
#include "../nested_payload_recoder.hpp"
#include "../nested_feedback_reader.hpp"
#include "../disable_trace.hpp"
#include "../trace_linear_block_decoder.hpp"
#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"

namespace kodo
{

    /// @ingroup fec_stacks
    ///
    /// @brief Implementation of a complete RLNC sliding window decoder
    ///
    /// This configuration extends the on_the_fly_decoder with the
    /// possibility to generate feedback to the sliding_window_encoder
    /// which will remove packets which have already been (partially)
    /// decoded from the encoding. To see an example of how to use
    /// this feature there is an example in the
    /// kodo/examples/sliding_window which demonstrates the use fo the
    /// feedback API.
    template<class Field, class TraceTag = disable_trace>
    class sliding_window_decoder :
        public // Feedback API
               nested_feedback_reader<
               feedback_pivot_status_writer<
               final_feedback_writer<
               // Payload API
               partial_decoding_tracker<
               rank_symbol_decoding_status_updater<
               nested_payload_recoder<
               proxy_stack<proxy_args<>, sliding_window_recoding_stack,
               payload_rank_decoder<
               payload_decoder<
               // Codec Header API
               systematic_decoder<
               symbol_id_decoder<
               // Symbol ID API
               plain_symbol_id_reader<
               // Decoder API
               pivot_status_writer<
               common_decoder_layers<TraceTag,
               // Coefficient Storage API
               coefficient_storage_layers<
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
               sliding_window_decoder<Field, TraceTag>
               > > > > > > > > > > > > > > > > > > > > >
    { };

}
