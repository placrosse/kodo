// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "rebind_factory.hpp"

namespace kodo
{
    /// @todo test + docs
    template<class SuperCoder>
    class is_complete_callback : public SuperCoder
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

        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t *payload)
        {
            SuperCoder::decode(payload);
            invoke_on_completion();
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data,
                           uint8_t *symbol_coefficients)
        {
            SuperCoder::decode_symbol(symbol_data, symbol_coefficients);
            invoke_on_completion();
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            SuperCoder::decode_symbol(symbol_data, symbol_index);
            invoke_on_completion();
        }

    private:

        /// This function will invoke the callback
        void invoke_on_completion()
        {
            if (!SuperCoder::is_complete())
                return;

            if (!m_callback)
                return;

            if (!m_invoked)
            {
                m_invoke = true;
                m_callback();
            }
        }

    private:

        /// Keeps track of whether we already invoked the callback so
        /// that we only do it once
        bool m_invoked;
    };

    template<class Codec>
    using wrap_restore_partial_symbol_decoder =
        wrap<restore_partial_symbol_decoder, Codec>;

}
