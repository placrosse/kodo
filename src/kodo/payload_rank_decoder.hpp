// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <algorithm>
#include <cstdint>

#include "rank_reader.hpp"
#include "payload_rank_reader.hpp"

namespace kodo
{

    /// @ingroup payload_codec_layers state_layers
    ///
    /// @brief The payload rank decoder combines the
    ///        payload_rank_reader and the rank_reader layers which
    ///        will expose the read rank using the
    ///        layer::remote_rank() const function.
    template<class SuperCoder>
    using payload_rank_decoder = payload_rank_reader<rank_reader<SuperCoder>>;

}
