// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @brief Provides functions to access and manipulate values in
    ///        an array of coefficients
    template<class SuperCoder>
    class coefficient_value_access : public SuperCoder
    {
    public:

        /// The field type used
        typedef typename SuperCoder::field_type field_type;

        /// The value type
        typedef typename SuperCoder::value_type value_type;

    public:

        /// Access the value of in an array of coefficients
        /// @param coefficients The coefficients array
        /// @param index The index of the value we are interested in
        /// @return The value at the specified index
        value_type coefficient_value(const value_type* coefficients,
                                     uint32_t index) const
        {
            assert(coefficients != 0);
            return fifi::get_value<field_type>(coefficients, index);
        }

        /// Set a value in an array of coefficients
        /// @param coefficients The coefficients array
        /// @param index The index in the array where the value should be set
        /// @param value The value to set in the array
        void set_coefficient_value(value_type* coefficients, uint32_t index,
                                   value_type value) const
        {
            assert(coefficients != 0);
            fifi::set_value<field_type>(coefficients, index, value);
        }

    };

}


