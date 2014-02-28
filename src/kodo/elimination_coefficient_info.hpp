// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @ingroup coefficient_storage_layers
    ///
    /// @brief The layer adjusts the size of the coding vectors which
    ///        allows non-square decoding matrices
    ///
    /// This layer is inserted into a decoding stack to effectively
    /// be able to create a non-square decoding matrix - we want
    /// to construct a decoder which more columns than rows. In the
    /// following example we have matrix C which is a 3x6 matrix, we
    /// can consider this as two 3x3 sub-matrices:
    ///
    ///       C
    ///  <--------->
    ///    A     B
    ///  <---> <--->
    ///  0 1 1 1 0 0
    ///  0 0 1 0 1 0
    ///  1 1 0 0 0 1
    ///
    /// In this case we consider the matrix A as the expansion however
    ///
    ///
    /// You will notice that matrix B is full rank and can be used to
    /// eliminate the three last coefficients of any incoming encoding
    /// vector.
    template<class SuperCoder>
    class elimination_coefficient_info : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// Constructor
        elimination_coefficient_info()
            : m_coefficients_length(0),
              m_coefficients_size(0)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            // The total number of elements in the coefficient vector
            // will be the elimination offset + one per symbol we are
            // actually decoding.
            auto total_elements =
                the_factory.symbols() + the_factory.elimination_offset();

            // We cannot have more elements than what is supported by the
            // decoder
            assert(total_elements <= the_factory.max_symbols());

            m_coefficients_length =
                fifi::elements_to_length<field_type>(total_elements);

            m_coefficients_size =
                fifi::elements_to_size<field_type>(total_elements);

            assert(m_coefficients_length > 0);
            assert(m_coefficients_size > 0);
        }

        /// @copydoc layer::coefficient_vector_length() const
        uint32_t coefficient_vector_length() const
        {
            assert(m_coefficients_length > 0);
            return m_coefficients_length;
        }

        /// @copydoc layer::coefficient_vector_size() const
        uint32_t coefficient_vector_size() const
        {
            assert(m_coefficients_size > 0);
            return m_coefficients_size;
        }

    private:

        /// The length of coefficients in value_type elements
        uint32_t m_coefficients_length;

        /// The size of coefficients in bytes
        uint32_t m_coefficients_size;
    };

}


