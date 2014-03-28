// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @todo docs
    /// @ingroup payload_codec_layers
    /// @brief Implements the recode function for Network Codes.
    ///
    /// The payload recoder layer has been implemented as
    /// a simple layer forwarding calls to recode to a proxy
    /// encoder. The reason for this design was that recoding could
    /// be implemented by almost entirely copying the layers of an
    /// encoder. The only difference being that the a special Symbol ID
    /// layer generating the recoding coefficients and creating the
    /// recoded symbol id (or encoding vector).
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


