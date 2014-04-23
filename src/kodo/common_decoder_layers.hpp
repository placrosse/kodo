// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "trace_linear_block_decoder.hpp"
#include "aligned_coefficients_decoder.hpp"
#include "forward_linear_block_decoder.hpp"
#include "rank_info.hpp"
#include "symbol_decoding_status_counter.hpp"
#include "symbol_decoding_status_tracker.hpp"

namespace kodo
{

    /// @ingroup decoder_layers
    ///
    /// @brief Template alias for a common selection of layers
    ///        full-filling the requirements for the Decoder API
    ///
    template<class TraceTag, class SuperCoder>
    using common_decoder_layers =
               trace_linear_block_decoder<TraceTag,
               aligned_coefficients_decoder<
               forward_linear_block_decoder<
               rank_info<
               symbol_decoding_status_counter<
               symbol_decoding_status_tracker<SuperCoder> > > > > >;

}
