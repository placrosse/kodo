// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @todo docs
    /// @ingroup coefficient_generator_layers
    /// @brief The storage aware generator will ensure that
    ///        layer::generate_partial(uint8_t*) is called whenever the encoder
    ///        indicates that it does not have full rank (i.e. not all symbols
    ///        have been specified).
    template<class SuperCoder>
    class remote_pivot_aware_generator : public SuperCoder
    {
    public:

        /// If the remote rank is larger than zero then we will not be
        /// able to use the layer::generate(uint8_t*) function to
        /// create the coding coefficients, since we will want to
        /// "prune" out the symbols which have already been received
        /// by the remote decoder.
        /// @copydoc layer::can_generate() const
        bool can_generate() const
        {
            if(SuperCoder::remote_rank() == 0)
            {
                // Let the SuperCoder layer determine whether to use the
                // layer::generate(uint8_t*) function
                return SuperCoder::can_generate();
            }
            else
            {
                return false;
            }
        }

        /// If the symbol is already pivot at the remote, then we
        /// don't want to generate it. Otherwise we ask the
        /// SuperCoder.
        bool can_generate(uint32_t index) const
        {
            if(SuperCoder::remote_is_symbol_pivot(index))
            {
                return false;
            }
            else
            {
                return SuperCoder::can_generate(index);
            }
        }

    };
}


