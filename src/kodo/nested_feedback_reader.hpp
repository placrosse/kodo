// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup feedback_layers
    ///
    /// @brief Implements and forward the layer::read_feedback(const
    ///        uint8_t*) function to the nested stack.
    ///
    /// This layer is useful if the read feedback call should be
    /// forwarded to a nested stack instead of being handled in the
    /// main stack.
    template<class SuperCoder>
    class nested_feedback_reader : public SuperCoder
    {
    public:

        /// The factory_base layer
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory_base::max_feedback_size() const
            uint32_t max_feedback_size() const
            {
                // We return the maximum size returned by either the
                // main stack or the nested stack
                auto& n = SuperCoder::nested();
                return std::max(SuperCoder::factory_base::max_feedback_size(),
                                n.max_feedback_size());
            }
        };


        /// @copydoc layer::read_feedback(const uint8_t*)
        void read_feedback(const uint8_t* feedback)
        {
            auto& n = SuperCoder::nested();
            assert(n);

            n->read_feedback(feedback);
        }

        /// Make sure we have enough space for both the payload
        /// produced by the main stack and the nested recoding stack.
        /// @copydoc layer::payload_size() const
        uint32_t feedback_size() const
        {
            auto& n = SuperCoder::nested();
            assert(n);

            // Return the maximum required feedback buffer size by
            // either the main stack or the nested stack
            return std::max(SuperCoder::feedback_size(), n->feedback_size());
        }

    };

}
