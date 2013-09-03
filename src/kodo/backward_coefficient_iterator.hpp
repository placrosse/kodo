// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <algorithm>

namespace kodo
{

    /// Policy object for determining the "direction" of the
    /// linear_block_decoder e.g. whether we will look for the
    /// first pivot from the beginning of the coefficients vector
    /// or in some other way.
    ///
    /// The backward_linear_block decoder will search for pivots from
    /// the right of the encoding vector.
    /// E.g. if you see an encoding vector like:
    ///
    ///   <-----------+ direction of search for pivots
    ///
    ///   0 1 0 1 1 0 0
    ///   ^       ^   ^
    ///   |       |   |
    ///   |       |   +----+  First coefficient searched
    ///   |       +--------+  First non zero coefficient
    ///   +----------------+  Last coefficient searched
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

        class iterator
        {

            /// @param
            iterator(const uint8_t* coefficients, uint32_t elements)
                : m_coefficients(coeffcients),
                  m_elements(elements),
                  m_index(elements)
            {
                assert(m_coefficients != 0);
                assert(m_elements > 0);
            }

            /// @return true if the policy is at the end
            bool at_end() const
            {
                return m_index == 0;
            }

            /// Advance the policy to the next index
            void advance()
            {
                assert(!at_end());
                --m_index;
            }

            /// @return the current index
            uint32_t index() const
            {
                return m_index - 1;
            }

            value_type value() const
            {
                return fifi::get_value<field_type>(m_coefficients, index());
            }

            /// The coefficients buffer
            const uint8_t* m_coefficients;

            /// The number of elements stored in the coefficients buffer
            uint32_t m_elements;

            /// The current element
            uint32_t m_index;

        };

    public:

        iterator coefficient_iterator(const uint8_t* coeffcients) const
        {
            return iterator(coefficients, SuperCoder::symbols());
        }

    };


}

