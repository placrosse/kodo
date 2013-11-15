// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    /// @brief The storage aware generator will ensure that
    ///        layer::generate_partial(uint8_t*) is called whenever the encoder
    ///        indicates that it does not have full rank (i.e. not all symbols
    ///        have been specified).
    template<class SuperCoder>
    class pivot_aware_generator : public SuperCoder
    {
    public:

        /// @todo
        bool can_generate() const
        {
            return SuperCoder::rank() == SuperCoder::symbols();
        }

        bool can_generate(uint32_t index) const
        {
            return SuperCoder::is_symbol_pivot(index);
        }

    };
}


