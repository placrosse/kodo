// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @ingroup payload_codec_layers
    ///
    /// @brief The payload_precoder layer checks when encoding whether
    ///        the SuperCoder::precode() function has been called if
    ///        not then it will invoke it.
    template<class SuperCoder>
    class payload_precoder : public SuperCoder
    {
    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_precoding_complete = false;
        }

        /// @copydoc layer::encode(uint8_t*)
        uint32_t encode(uint8_t *payload)
        {
            if(!m_precoding_complete)
            {
                SuperCoder::precode();
                m_precoding_complete = true;
            }

            return SuperCoder::encode(payload);
        }

    protected:

        /// Tracks whether the precoding has been completed
        bool m_precoding_complete;

    };
}
