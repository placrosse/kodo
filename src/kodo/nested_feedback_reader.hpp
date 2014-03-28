// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @todo docs
    template<class SuperCoder>
    class nested_feedback_reader : public SuperCoder
    {
    public:

        /// The factory layer
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_feedback_size() const
            uint32_t max_feedback_size() const
            {
                // We return the maximum size returned by either the
                // main stack or the nested stack
                auto& n = SuperCoder::nested();
                return std::max(SuperCoder::factory::max_feedback_size(),
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


