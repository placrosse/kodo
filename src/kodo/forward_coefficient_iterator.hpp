// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @todo Update class documentation
    /// Policy object for determining the "direction" of the
    /// linear_block_decoder e.g. whether we will look for the
    /// first pivot from the beginning of the coefficients vector
    /// or in some other way.
    ///
    /// The forward_linear_block decoder will search for pivots from
    /// the left of the encoding vector.
    /// E.g. if you see an encoding vector like:
    ///
    ///   +-----------> direction of search for pivots
    ///
    ///   0 1 0 1 1 0 0
    ///   ^ ^         ^
    ///   | |         |
    ///   | |         +----+  Last coefficient searched
    ///   | +--------------+  First non zero coefficient
    ///   +----------------+  First coefficient searched
    ///
    template<class SuperCoder>
    class forward_coefficient_iterator : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// Nested iterator type which encapsulates the iteration
        /// logic though the coding coefficients.
        class iterator
        {
        public:

            /// @param coefficients The coding coefficients
            /// @param elements The number of coefficients stored in the
            ///        coefficients buffer.
            /// @param start_index A starting index which allows us to
            ///        offset the iterator to start at a specific index
            iterator(const uint8_t* coefficients,
                     uint32_t elements,
                     uint32_t start_index)
                : m_coefficients(coefficients),
                  m_elements(elements),
                  m_index(start_index)
            {
                assert(m_coefficients != 0);
                assert(m_elements > 0);
                assert(m_index < m_elements);
            }

            /// @return true if the policy is at the end
            bool at_end() const
            {
                return m_index >= m_elements;
            }

            /// Advance the policy to the next index
            void advance()
            {
                assert(!at_end());
                ++m_index;
            }

            /// @return The current index
            uint32_t index() const
            {
                return m_index;
            }

            /// @return The value of the coefficient at the current index
            value_type value() const
            {
                return fifi::get_value<field_type>(m_coefficients, m_index);
            }

        private:

            /// The coefficients buffer
            const uint8_t* m_coefficients;

            /// The number of elements stored in the coefficients buffer
            uint32_t m_elements;

            /// The current element
            uint32_t m_index;

        };

    public:

        /// @param coefficients The buffer to iterate over
        /// @return The iterator object
        iterator coefficient_iterator(const uint8_t* coefficients) const
        {
            assert(coefficients != 0);
            return iterator(coefficients, SuperCoder::symbols(), 0);
        }

        /// @param coefficients The buffer to iterate over
        /// @param start_index The index from which iteration should start
        /// @return The iterator object
        iterator coefficient_iterator(const uint8_t* coefficients,
                                      uint32_t start_index) const
        {
            assert(coefficients != 0);
            assert(start_index < SuperCoder::symbols());
            return iterator(coefficients, SuperCoder::symbols(), start_index);
        }


    };

}

