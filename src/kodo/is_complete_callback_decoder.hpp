// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "rebind_factory.hpp"

namespace kodo
{
    /// @ingroup payload_codec_layers
    ///
    /// @brief Checks whether the layer::is_complete() status changes
    ///        after calling layer::decode(uint8_t*) if
    ///        layer::is_complete() returns true the callback will be
    ///        invoked.
    ///
    /// The callback will only be invoked once.
    template<class SuperCoder>
    class is_complete_callback_decoder : public SuperCoder
    {
    public:

        /// The completion callback type
        using callback = std::function<void()>;

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_invoked = false;
            m_callback = nullptr;
        }

        /// Sets the callback to be invoked once decoding is complete
        ///
        /// @param callback The callback that we should invoke when
        ///        layer::is_complete changes state to true
        void set_is_complete_callback(const callback& callback)
        {
            assert(!SuperCoder::is_complete());
            assert(!m_invoked);
            assert(!m_callback);
            m_callback = callback;
        }

        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t *payload)
        {
            SuperCoder::decode(payload);

            if (!SuperCoder::is_complete())
            {
                return;
            }

            if (!m_callback)
            {
                return;
            }

            if (!m_invoked)
            {
                m_invoked = true;
                m_callback();
            }
        }

    private:

        /// Keeps track of whether we already invoked the callback so
        /// that we only do it once
        bool m_invoked;

        /// The callback we will invoke once decoding is complete
        callback m_callback;
    };
}
