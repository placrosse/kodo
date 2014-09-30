// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <fifi/fifi_utils.hpp>

#include "vandermonde_matrix.hpp"

namespace kodo
{

    /// @brief Computes a Vandermonde matrix to generate the coding
    ///        coefficients. The matrix code is
    template<class SuperCoder>
    class systematic_vandermonde_matrix_base : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

        /// The generator matrix type
        typedef typename SuperCoder::generator_matrix generator_matrix;

    public:

        /// The factory layer associated with this coder. Maintains
        /// the block generator needed for the encoding vectors.
        class factory_base : public SuperCoder::factory_base
        {
        protected:

            /// Access to the finite field implementation used stored in
            /// the finite_field_math layer
            using SuperCoder::factory_base::m_field;

        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t, uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size);

            /// Constructs systematic Vandermonde matrix.
            /// @param symbols The number of source symbols to encode
            /// @return The Vandermonde matrix
            std::shared_ptr<generator_matrix> construct_matrix(
                uint32_t symbols);

        };

    };

    template<class SuperCoder>
    systematic_vandermonde_matrix_base<SuperCoder>::factory_base::factory_base(
        uint32_t max_symbols, uint32_t max_symbol_size)
        : SuperCoder::factory_base(max_symbols, max_symbol_size)
    {
        // A Reed-Solomon code cannot support more symbols
        // than 2^m - 1 where m is the size of the finite
        // field
        assert(max_symbols < field_type::order);
    }

    template<class SuperCoder>
    inline auto
    systematic_vandermonde_matrix_base<SuperCoder>::factory_base::construct_matrix(
        uint32_t symbols) -> std::shared_ptr<generator_matrix>
    {
        assert(symbols > 0);
        assert(m_field);

        std::shared_ptr<generator_matrix> m =
            SuperCoder::factory_base::construct_matrix(symbols);
        assert(m);

        std::vector<value_type> temp_row(m->row_length());

        for(uint32_t i = 0; i < m->rows(); ++i)
        {
            value_type pivot = m->element(i,i);
            pivot = fifi::pack_constant<field_type>(m_field->invert(pivot));

            m_field->region_multiply_constant(m->row_value(i), pivot, m->row_length());

            for(uint32_t j = 0; j < m->rows(); ++j)
            {
                if(j == i)
                    continue;

                value_type scale = fifi::pack_constant<field_type>(m->element(j, i));

                m_field->region_multiply_subtract(m->row_value(j),
                    m->row_value(i), scale, m->row_length());

            }
        }

        return m;

    }

}
