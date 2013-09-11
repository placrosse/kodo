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
    /// The forward_coefficient_iterator will iterate through coefficients
    /// from front to back e.g.:
    ///
    ///   +-----------> direction of iteration
    ///
    ///   0 1 0 1 1 0 0
    ///   ^           ^
    ///   |           |
    ///   |           +----+  Last coefficient
    ///   |
    ///   +----------------+  First coefficient in iteration
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
        /// logic though the coding coefficients. With indexes:
        /// {0;elements-1}
        class iterator
        {
        public:

            /// @param coefficients The coding coefficients
            /// @param elements The number of coefficients stored in the
            ///        coefficients buffer.
            /// @param start_index A starting index which allows us to
            ///        offset the iterator to start at a specific index
            iterator(const uint8_t* coefficients,
                     uint32_t start_index,
                     uint32_t stop_index)
                : m_coefficients(coefficients),
                  m_start_index(start_index),
                  m_stop_index(stop_index),
                  m_offset(0)
            {
                assert(m_coefficients != 0);
                assert(m_start_index <= m_stop_index);
            }

            /// @return true if the iterator is at the end
            bool at_end() const
            {
                return m_start_index + m_offset > m_stop_index;
            }

            /// Advance the iterator to the next index
            void advance()
            {
                assert(!at_end());
                ++m_offset;
            }

            /// @return The current index
            uint32_t index() const
            {
                return m_start_index + m_offset;
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
                return std::max(a,b);
            }

            /// The minimum index depends on the direction of the iterator
            /// e.g. for a forward iterator index 0 will be the smallest
            /// @param a The first index
            /// @param b The second index
            /// @return The minimum value of the two indexes
            static uint32_t min_index(uint32_t a, uint32_t b)
            {
                return std::min(a,b);
            }

        private:

            /// The coefficients buffer
            const uint8_t* m_coefficients;

            /// The number of elements stored in the coefficients buffer
            uint32_t m_start_index;

            /// The current element
            uint32_t m_stop_index;

            ///
            uint32_t m_offset;

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

