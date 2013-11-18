// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "rank_reader.hpp"
#include "payload_rank_reader.hpp"

namespace kodo
{

    /// @todo docs
    /// @ingroup payload_codec_layers
    /// @brief The payload decoder splits the payload buffer into
    ///        symbol header and symbol.
    template<class SuperCoder>
    class payload_rank_decoder :
        public payload_rank_reader<rank_reader<SuperCoder> >
    { };

}


