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

        /// Nested iterator type which encapsulates the iteration
        /// logic though the coding coefficients. With indexes:
        /// {0; elements-1}
        class iterator
        {
        public:

            /// @param elements The number of coefficients stored in the
            ///        coefficients buffer.
            /// @param start_index A starting index which allows us to
            ///        offset the iterator to start at a specific index
            iterator(uint32_t start_index, uint32_t stop_index)
                : m_start_index(start_index),
                  m_stop_index(stop_index)
            {
                assert(m_start_index <= m_stop_index);
            }

            /// @return true if the iterator is at the end
            bool at_end() const
            {
                return m_start_index > m_stop_index;
            }

            /// Advance the iterator to the next index
            void advance()
            {
                assert(!at_end());
                ++m_start_index;
            }

            /// @return The current index
            uint32_t index() const
            {
                return m_start_index;
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

        protected:

            /// The number of elements stored in the coefficients buffer
            uint32_t m_start_index;

            /// The current element
            uint32_t m_stop_index;

        };

    public:

        /// @param coefficients The buffer to iterate over
        /// @return The iterator object
        iterator coefficient_iterator() const
        {
            return iterator(0U, SuperCoder::symbols() - 1);
        }

        /// @param coefficients The buffer to iterate over
        /// @param start_index The index from which iteration should start
        /// @return The iterator object
        iterator coefficient_iterator(uint32_t start_index) const
        {
            assert(start_index < SuperCoder::symbols());
            return iterator(start_index, SuperCoder::symbols() - 1);
        }


    };

}

