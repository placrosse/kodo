// Copyright Steinwurf ApS 2011-2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <vector>

namespace kodo
{
    /// @todo test + docs
    ///
    /// Layer used by the fulcrum outer decoder to perform a direct
    /// mapping from the inner to the outer code
    template<class SuperCoder>
    class fulcrum_outer_symbol_mapper : public SuperCoder
    {
    public:

        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            // Prepare buffer for the coefficients of the outer code
            m_temp_coefficients.resize(
                the_factory.max_coefficient_vector_size());
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            assert(symbol_data != 0);
            assert(coefficients != 0);

            SuperCoder::map_to_outer(coefficients, m_temp_coefficients.data());
            SuperCoder::decode_symbol(symbol_data, m_temp_coefficients.data());
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            assert(symbol_index < SuperCoder::inner_symbols());
            assert(symbol_data != 0);

            if(SuperCoder::is_outer_systematic())
            {
                if(symbol_index < SuperCoder::symbols())
                {
                    SuperCoder::decode_symbol(symbol_data, symbol_index);
                    return;
                }
            }

            SuperCoder::map_to_outer(symbol_index, m_temp_coefficients.data());
            SuperCoder::decode_symbol(symbol_data, m_temp_coefficients.data());
        }

    private:

        /// Coefficients use for the outer code
        std::vector<uint8_t> m_temp_coefficients;
    };
}
