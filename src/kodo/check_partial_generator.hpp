// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    /// @brief The storage aware generator will ensure that
    ///        layer::generate_partial(uint8_t*) is called whenever the encoder
    ///        indicates that it does not have full rank (i.e. not all symbols
    ///        have been specified).
    template<class SuperCoder>
    class check_partial_generator : public SuperCoder
    {
    public:

        /// @copydoc layer::generate(uint8_t*)
        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            if(SuperCoder::can_generate())
            {
                SuperCoder::generate(coefficients);
            }
            else
            {
                SuperCoder::generate_partial(coefficients);
            }
        }
    };

}
