// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup feedback_layers
    ///
    /// @brief This layer terminates a series of
    ///        layer::factory_base::max_feedback_size() const and
    ///        layer::feedback_size() const calls.
    template<class SuperCoder>
    class final_feedback_size : public SuperCoder
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
                return 0;
            }
        };

    public:

        /// @copydoc layer::feedback_size() const
        uint32_t feedback_size() const
        {
            return 0;
        }

    };

}
