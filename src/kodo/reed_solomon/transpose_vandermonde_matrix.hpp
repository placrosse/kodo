// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/make_shared.hpp>

namespace kodo
{

    /// @brief Construct the transposes the Vandermonde matrix in order to
    ///        make memory access more efficient
    template<class SuperCoder>
    class transpose_vandermonde_matrix : public SuperCoder
    {
    public:

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

        /// The generator matrix type
        typedef typename SuperCoder::generator_matrix generator_matrix;

    public:

        /// The factory layer associated with this coder. Maintains
        /// the block generator needed for the encoding vectors.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t, uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }

            /// Transposes the constructed Vandermonde matrix
            /// @param symbols The number of source symbols to encode
            /// @return The Vandermonde matrix
            std::shared_ptr<generator_matrix> construct_matrix(
                uint32_t symbols)
            {
                auto matrix =
                    SuperCoder::factory_base::construct_matrix(symbols);

                assert(matrix);

                return std::make_shared<generator_matrix>(
                    matrix->transpose());
            }

        };

    };
}
