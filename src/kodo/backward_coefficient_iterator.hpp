// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @brief Iterator object determining the "direction" we iterate through
    /// a vector of coding coefficients.
    ///
    /// The backward_coefficient_iterator will iterate through coefficients
    /// from back to front e.g.:
    ///
    ///   <-----------+ direction of iteration
    ///
    ///   0 1 0 1 1 0 0
    ///   ^           ^
    ///   |           |
    ///   |           +----+  First coefficient in iteration
    ///   |
    ///   +----------------+  Last coefficient
    ///
    template<class SuperCoder>
    class backward_coefficient_iterator : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// Nested iterator type which encapsulates the iteration
        /// logic though the coding coefficients. With indexes:
        /// {0;elements-1}
        class iterator_type
        {
        public:

            /// @param coefficients The coding coefficients
            /// @param elements The number of coefficients stored in the
            ///        coefficients buffer.
            /// @param start_index A starting index which allows us to
            ///        offset the iterator to start at a specific index
            iterator_type(const uint8_t* coefficients,
                          uint32_t start_index,
                          uint32_t stop_index)
                : m_coefficients(coefficients),
                  m_start_index(start_index),
                  m_stop_index(stop_index),
                  m_offset(0)
            {
                assert(m_coefficients != 0);
                assert(m_start_index >= m_stop_index);
            }

            /// @return true if the iterator is at the end
            bool at_end() const
            {
                return m_stop_index + m_offset > m_start_index;

            }

            /// Advance the iterator to the next index
            void advance()
            {
                assert(!at_end());
                ++m_offset;
            }

            /// @return the current index
            uint32_t index() const
            {
                return m_start_index - m_offset;
            }

            /// @return The value of the coefficient at the current index
            value_type value() const
            {
                return fifi::get_value<field_type>(m_coefficients, index());
            }

            /// The maximum index depends on the direction of the iterator
            /// e.g. for a backward iterator index 0 will be the largest
            /// @param a The first index
            /// @param b The second index
            /// @return The maximum value of the two indexes
            static uint32_t max_index(uint32_t a, uint32_t b)
            {
                return std::min(a,b);
            }

            /// The minimum index depends on the direction of the iterator
            /// e.g. for a forward iterator index 0 will be the smallest
            /// @param a The first index
            /// @param b The second index
            /// @return The minimum value of the two indexes
            static uint32_t min_index(uint32_t a, uint32_t b)
            {
                return std::max(a,b);
            }

        private:

            /// The coefficients buffer
            const uint8_t* m_coefficients;

            /// The number of elements stored in the coefficients buffer
            uint32_t m_start_index;

            /// The current element
            uint32_t m_stop_index;

            /// The offset from the stop index
            uint32_t m_offset;

        };

    public:

        /// @param coefficients The buffer to iterate over
        /// @return The iterator object
        iterator_type coefficient_iterator(const uint8_t* coefficients) const
        {
            assert(coefficients != 0);

            auto from = iterator_type::min_index(0, SuperCoder::symbols() - 1);
            auto to = iterator_type::max_index(0, SuperCoder::symbols() - 1);

            return iterator_type(coefficients, from, to);
        }

        /// @param coefficients The buffer to iterate over
        /// @param start_index The index from which iteration should start
        /// @return The iterator object
        iterator_type coefficient_iterator(const uint8_t* coefficients,
                                           uint32_t start_index,
                                           uint32_t stop_index) const
        {
            assert(coefficients != 0);
            assert(start_index < SuperCoder::symbols());
            return iterator_type(coefficients, start_index, stop_index);
        }

    };


}

