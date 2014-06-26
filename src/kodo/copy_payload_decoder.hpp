// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>

namespace kodo
{
    /// @ingroup payload_codec_layers
    ///
    /// @brief Make decoder work on a copy of the payload
    ///
    /// In the standard API the payload buffer may be modified during
    /// decoding, this can be problematic if you wish to e.g. pass the
    /// same payload to multiple decoders. To solve this you may use
    /// the copy_payload_decoder layer to ensure that the payload is
    /// not modified by decoders.
    template<class SuperCoder>
    class copy_payload_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_payload_copy.resize(the_factory.max_payload_size(), 0);
        }

        /// Copy the payload data to ensure that the payload isn't
        /// overwritten during decoding
        ///
        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t *payload)
        {
            assert(payload != 0);

            /// Copy payload to m_payload_copy
            std::copy_n(payload, SuperCoder::payload_size(),
                        m_payload_copy.data());

            SuperCoder::decode(m_payload_copy.data());
        }

    protected:

        /// Copy of payload
        std::vector<uint8_t> m_payload_copy;
    };
}
