// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "coefficient_value_access.hpp"

namespace kodo
{

    /// @brief Provides functions to access and manipulate values in
    ///        an array of coefficients, but with an added offset to the
    ///        indexes.
    ///
    /// This is useful in cases where we want to create e.g. an non-square
    /// decoding matrix. E.g. imagine this matrix:
    ///
    ///    1 0 1 0 1 1
    ///    0 1 1 1 0 0
    ///    0 1 0 1 0 1
    ///
    /// By default we will always access the coding coefficients from the
    /// beginning of the encoding vector. Which enables us to decode e.g.
    /// a follows:
    ///
    ///    1 0 0 0 1 0
    ///    0 1 0 1 0 1
    ///    0 0 1 0 0 1
    ///
    /// This has nicely create 3 pivot positions from in the first 3
    /// columns.  Now, lets imagine that we wanted to shift those
    /// pivot positions so that they would appear in the last three
    /// columns. This could be done by adding a offset to the index
    /// when inspecting the coefficient values.  I.e. this means that
    /// when the decoding algorithm access the coefficient at index 0
    /// this layer will adjust that to mean e.g. index 3 and so
    /// forth. This ofcourse requires that we also accommodate this in
    /// other layers e.g. when allocating storage for the coding
    /// coefficients. For more info on this see the
    /// elimination_coefficient_info and
    /// elimination_coefficient_offset layers.
    template<class SuperCoder>
    class elimination_coefficient_value_access : public SuperCoder
    {
    public:

        /// @copydoc layer::value_type;
        typedef typename SuperCoder::value_type value_type;

    public:

        /// @copydoc coefficient_value_access::coefficient_value(
        ///             const value_type*, uint32_t) const
        value_type coefficient_value(const value_type* coefficients,
                                     uint32_t index) const
        {
            assert(index < SuperCoder::symbols());
            index += SuperCoder::elimination_offset();

            return SuperCoder::coefficient_value(coefficients, index);
        }

        /// @copydoc coefficient_value_access::coefficient_value(
        ///             const value_type*, uint32_t, value_type) const
        void set_coefficient_value(value_type* coefficients, uint32_t index,
                                   value_type value) const
        {
            assert(index < SuperCoder::symbols());
            index += SuperCoder::elimination_offset();

            SuperCoder::set_coefficient_value(coefficients, index, value);
        }

    };

}
