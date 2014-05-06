// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @todo test + docs
    ///
    /// The nested payload decoder layer redirects all calls to
    /// the layer::decode(uint8_t*) function to a nested stack.
    /// In addition to forwarding the layer::decode(uint8_t*)
    /// function the layer also forwards the
    /// layer::factory::max_payload_size() const and layer::payload_size() const
    /// functions to ensure that buffers allocated by the user has the proper
    /// sizes.
    ///
    /// On common use-case for this type of layer are situations where a
    /// nested stack should e.g. "preprocess" the incoming symbols.
    template<class SuperCoder>
    class nested_payload_decoder : public SuperCoder
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
            /// @todo should be max between SuperCoder and nested
            ///       same goes for the factory
            assert(SuperCoder::nested());
            return SuperCoder::nested()->payload_size();
        }

        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t *payload)
        {
            assert(payload);
            assert(SuperCoder::nested());
            SuperCoder::nested()->decode(payload);
        }
    };
}
