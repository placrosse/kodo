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
#include <kodo/backward_linear_block_decoder_policy.hpp>

namespace kodo
{

    /// @ingroup decoder_layers @brief Implements basic linear block
    /// decoder which uses the backward policy. This means that the
    /// Gaussian Elimination is performed beginning from the last
    /// column instead of typically the first.
    template<class SuperCoder>
    class backward_linear_block_decoder :
        public bidirectional_linear_block_decoder<
            backward_linear_block_decoder_policy, SuperCoder>
    { };

}
