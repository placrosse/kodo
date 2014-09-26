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
    /// @brief The feedback_pivot_status provides a building block for
    ///        the feedback_pivot_status_reader and
    ///        feedback_pivot_status_writer layers by implementing the
    ///        part of the feedback API which whould otherwise have to
    ///        be duplicated.
    template<class SuperCoder>
    class feedback_pivot_status : public SuperCoder
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
                return SuperCoder::factory_base::max_feedback_size() +
                    SuperCoder::factory_base::max_pivot_status_size();
            }
        };

    public:

        /// @copydoc layer::feedback_size() const
        uint32_t feedback_size() const
        {
            return SuperCoder::feedback_size() +
                SuperCoder::pivot_status_size();
        }
    };
}
