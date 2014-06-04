// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup payload_codec_layers
    ///
    /// @brief Implements the recode function for Network Codes.
    ///
    /// The nested_payload_recoder layer has been implemented as a
    /// simple layer forwarding calls to recode to a nested/proxy
    /// stack. The reason for this design was that recoding can be
    /// implemented by almost entirely copying the layers of an
    /// encoder. However, to avoid mixin encoding and decoding layers
    /// the encoding layers are typically place in a nested
    /// stack. This layer forwards the recode call to that stack.
    template<class SuperCoder>
    class nested_payload_recoder : public SuperCoder
    {
    public:

        /// @copydoc layer::recode(uint8_t*)
        uint32_t recode(uint8_t *payload)
        {
            auto& recoder = SuperCoder::nested();
            assert(recoder);

            return recoder->recode(payload);
        }

        /// Make sure we have enough space for both the payload
        /// produced by the main stack and the nested recoding stack.
        /// @copydoc layer::payload_size() const
        uint32_t payload_size() const
        {
            auto& recoder = SuperCoder::nested();
            assert(recoder);

            return std::max(
                SuperCoder::payload_size(), recoder->payload_size());
        }

    };

}


