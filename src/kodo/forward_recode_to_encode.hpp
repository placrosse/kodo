// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup payload_codec_layers
    ///
    /// @brief Forwards class to layer::recode(uint8_t*) to
    ///        layer::encode(uint8_t*)
    ///
    /// This little helper is useful in the recoding stacks where the
    /// actual recoder is implemented mostly using the encoding
    /// layers.
    template<class SuperCoder>
    class forward_recode_to_encode : public SuperCoder
    {
    public:

        /// @copydoc layer::recode(uint8_t*)
        uint32_t recode(uint8_t* payload)
        {
            return SuperCoder::encode(payload);
        }

    };

}
