// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{

    /// @todo test + review + ingroup
    template<class SuperCoder>
    class nested_payload_encoder : public SuperCoder
    {
    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_payload_size() const
            uint32_t max_payload_size() const
            {
                return SuperCoder::factory::nested().max_payload_size();
            }
        };

        /// @copydoc layer::payload_size() const
        uint32_t payload_size() const
        {
            return SuperCoder::nested()->payload_size();
        }

        /// @copydoc layer::encode(uint8_t*)
        uint32_t encode(uint8_t *payload)
        {
            assert(payload);
            return SuperCoder::nested()->encode(payload);
        }

    };

}
