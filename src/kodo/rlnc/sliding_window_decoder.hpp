// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "on_the_fly_codes.hpp"
#include "sliding_window_systematic_encoder.hpp"

#include "../pivot_status_reader.hpp"
#include "../feedback_pivot_status_reader.hpp"
#include "../final_feedback_reader.hpp"

#include "../pivot_status_writer.hpp"
#include "../feedback_pivot_status_writer.hpp"
#include "../final_feedback_writer.hpp"

namespace kodo
{

    /// Intermediate stack implementing the recoding functionality of
    /// a RLNC code. As can be seen we are able to reuse a great deal
    /// of layers from the encode stack. It is important that the
    /// symbols produced by the recoder are compatible with the
    /// decoder. This means we have to use compatible Payload, Codec
    /// Header Symbol ID layers, between the encoder, recoder and
    /// decoder.  The only layer specific to recoding is the
    /// recoding_symbol_id layer. Finally the recoder uses a
    /// proxy_layer which forwards any calls not implemented in the
    /// recoding stack to the MainStack.
    template<class MainStack>
    class sliding_window_recoding_stack
        : public // Feedback API
                 feedback_pivot_status_reader<
                 final_feedback_reader<
                 // Payload API
                 payload_rank_recoder<
                 payload_encoder<
                 // Codec Header API
                 sliding_window_systematic_encoder<
                 symbol_id_encoder<
                 // Symbol ID API
                 recoding_symbol_id<
                 // Coefficient Generator API
                 uniform_generator<
                 pivot_aware_generator<
                 // Encoder API
                 encode_symbol_tracker<
                 zero_symbol_encoder<
                 linear_block_encoder<
                 pivot_status_reader<
                 rank_info<
                 // Coefficient Storage API
                 coefficient_value_access<
                 // Proxy
                 proxy_remote_rank<
                 proxy_layer<
                 sliding_window_recoding_stack<MainStack>,
                 MainStack> > > > > > > > > > > > > > > > >
    { };

    /// @ingroup fec_stacks
    /// @brief Implementation of a complete RLNC sliding window decoder
    ///
    /// This configuration extends the on_the_fly_decoder with the
    /// possibility to generate feedback to the sliding_window_encoder
    /// which will remove packets which have already been (partially)
    /// decoded from the encoding. To see an example of how to use
    /// this feature there is an example in the
    /// kodo/examples/sliding_window which demonstrates the use fo the
    /// feedback API.
    template<class Field>
    class sliding_window_decoder :
        public // Feedback API
               feedback_pivot_status_writer<
               final_feedback_writer<
               // Payload API
               partial_decoding_tracker<
               rank_symbol_decoding_status_updater<
               payload_recoder<sliding_window_recoding_stack,
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



