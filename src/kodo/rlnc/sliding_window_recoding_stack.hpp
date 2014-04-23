// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "full_rlnc_codes.hpp"
#include "on_the_fly_generator.hpp"
#include "sliding_window_systematic_encoder.hpp"
#include "sliding_window_generator.hpp"
#include "../pivot_status_reader.hpp"
#include "../feedback_pivot_status_reader.hpp"
#include "../final_feedback_reader.hpp"
#include "../partial_decoding_tracker.hpp"
#include "../rank_info.hpp"
#include "../payload_rank_encoder.hpp"
#include "../payload_rank_decoder.hpp"
#include "../payload_rank_recoder.hpp"
#include "../proxy_remote_rank.hpp"
#include "../coefficient_value_access.hpp"
#include "../rank_symbol_decoding_status_updater.hpp"
#include "../forward_recode_to_encode.hpp"

namespace kodo
{
    /// @todo ingroup
    ///
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
                 // Payload Codec API
                 forward_recode_to_encode<
                 payload_rank_recoder<
                 payload_encoder<
                 // Codec Header API
                 sliding_window_systematic_encoder<
                 symbol_id_encoder<
                 // Symbol ID API
                 recoding_symbol_id<
                 // Coefficient Generator API
                 sliding_window_generator<
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

}
