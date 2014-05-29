// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../coefficient_value_access.hpp"
#include "../default_off_systematic_encoder.hpp"
#include "../encode_symbol_tracker.hpp"
#include "../forward_recode_to_encode.hpp"
#include "../linear_block_encoder.hpp"
#include "../payload_encoder.hpp"
#include "../perpetual_generator.hpp"
#include "../perpetual_recoding_symbol_id.hpp"
#include "../pivot_aware_generator.hpp"
#include "../proxy_layer.hpp"
#include "../recoding_symbol_id.hpp"
#include "../symbol_id_encoder.hpp"
#include "../zero_symbol_encoder.hpp"

namespace kodo
{
    /// @ingroup fec_stacks
    ///
    /// @brief Intermediate stack implementing the recoding
    ///        functionality of the perpetual RLNC code.
    ///
    /// As can be seen we are able to reuse a great deal of
    /// layers from the encode stack. It is important that the symbols
    /// produced by the recoder are compatible with the decoder. This
    /// means we have to use compatible Payload, Codec Header Symbol ID
    /// layers, between the encoder, recoder and decoder.
    /// The only layer specific to recoding is the perpetual_recoding_symbol_id
    /// layer. Finally the recoder uses a proxy_layer which forwards
    /// any calls not implemented in the recoding stack to the MainStack.
    template<class MainStack>
    class perpetual_recoding_stack : public
        // Payload API
        forward_recode_to_encode<
        payload_encoder<
        // Codec Header API
        symbol_id_encoder<
        // Symbol ID API
        perpetual_recoding_symbol_id<
        // Coefficient Generator API
        perpetual_generator<
        pivot_aware_generator<
        // Encoder API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        // Proxy
        proxy_layer<
        perpetual_recoding_stack<MainStack>, MainStack>
        > > > > > > > > > >
    { };
}
