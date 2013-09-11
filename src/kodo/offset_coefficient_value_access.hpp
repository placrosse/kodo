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
    class offset_coefficient_value_access :
        public coefficient_value_access<SuperCoder>
    {
    public:

        /// copydoc
        value_type coefficient_value(const value_type* coefficients,
                                     uint32_t index) const
        {
            return Super::coefficient_value(coefficient, index + m_offset);
        }

        /// @copydoc
        void set_coefficient_value(value_type* coefficients, uint32_t index,
                                   value_type value) const
        {
            Super::set_coefficient_value(coefficients, index + m_offset, value);
        }

    };

}


