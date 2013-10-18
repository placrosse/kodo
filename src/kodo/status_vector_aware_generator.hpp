// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    /// @brief The decoded aware generator will ensure that
    ///        layer::generate_decoded_aware(uint8_t*) is called whenever the
    ///        encoder has information about the status of the decoder. This
    ///        will avoid the inclusion of symbols that the decoder does not
    ///        need.
    template<class SuperCoder>
    class status_vector_aware_generator : public SuperCoder
    {
    public:

        /// @copydoc layer::generate(uint8_t*)
        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            if(SuperCoder::decoder_symbols() > 0)
            {
                SuperCoder::generate_status_vector_aware(coefficients);
            }
            else if(SuperCoder::rank() < SuperCoder::symbols())
            {
                SuperCoder::generate_partial(coefficients);
            }
            else
            {
                SuperCoder::generate(coefficients);
            }
        }
    };
}


