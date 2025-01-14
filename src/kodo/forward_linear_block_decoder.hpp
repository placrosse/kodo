// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

#include <sak/storage.hpp>

#include <fifi/is_binary.hpp>
#include <fifi/fifi_utils.hpp>

#include <kodo/bidirectional_linear_block_decoder.hpp>
#include <kodo/forward_linear_block_decoder_policy.hpp>

namespace kodo
{

    /// @ingroup decoder_layers
    /// @brief Implements basic linear block
    /// decoder using the forward direction policy. This is equivalent
    /// to the standard approach to Gaussian Elimination.
    template<class SuperCoder>
    class forward_linear_block_decoder :
        public bidirectional_linear_block_decoder<
            forward_linear_block_decoder_policy, SuperCoder>
    { };

}
