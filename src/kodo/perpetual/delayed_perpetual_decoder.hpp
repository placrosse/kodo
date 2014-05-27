// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <algorithm>

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

namespace kodo
{

    /// Perpetual decoder which identifies the index most suitable to use as a
    /// pivot in order to best utilize the perpetual structure
    template<class SuperCoder>
    class delayed_perpetual_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// Constructor
        delayed_perpetual_decoder() :
            m_max_seen_width(0)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_max_seen_width = 0;
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            assert(symbol_index < SuperCoder::symbols());
            assert(symbol_data != 0);

            if (SuperCoder::is_symbol_decoded(symbol_index))
            {
                return;
            }

            const value_type *symbol =
                reinterpret_cast<value_type*>(symbol_data);

            if (SuperCoder::is_symbol_seen(symbol_index))
            {
                SuperCoder::swap_decode(symbol, symbol_index);
            }
            else
            {
                // Store the uncoded symbol
                SuperCoder::store_uncoded_symbol(symbol, symbol_index);
            }

            SuperCoder::decode_symbol(symbol_data, symbol_index);

            if (SuperCoder::is_complete())
                final_decoding();

            SuperCoder::update_symbol_status();
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            assert(symbol_data != 0);
            assert(coefficients != 0);

            value_type *s = reinterpret_cast<value_type*>(symbol_data);
            value_type *c = reinterpret_cast<value_type*>(coefficients);
            decode_coefficients(s, c);

            if (SuperCoder::is_complete())
                final_decoding();

            SuperCoder::update_symbol_status();
        }

    protected:

        /// the max width seen
        uint32_t m_max_seen_width;

        /// @return the highist width seen
        uint32_t max_width() const
        {
            return m_max_seen_width;
        }

        /// update the highest width seen
        /// @param width an observed width
        void seen_width(uint32_t width)
        {
            assert(width <= SuperCoder::symbols());
            if (width > m_max_seen_width)
                m_max_seen_width = width;
        }

        /// Find the intended pivot index in a perpetual coding vector, which
        /// is the index following the longest streak of zeros in the vector
        /// @param symbol_id the coding vector
        /// @return a pivot candidate if found
        boost::optional<uint32_t> find_pivot(value_type *symbol_id)
        {
            // as the longest zero streak may run over the end of the coding
            // vector we look through the vector from the beginning until we
            // have passed the end and run into a non-zero element. We handle
            // the two special cases that the vector is all zeros and all
            // non-zeros

            // if we receive a full vector the pivot and longest_streak will
            // not be updated
            uint32_t symbols = SuperCoder::symbols();
            boost::optional<uint32_t> pivot = boost::optional<uint32_t>(0);
            uint32_t longest_streak = 0;

            uint32_t zeros = 0;
            bool streak = false;
            uint32_t i = 0;
            value_type value;

            do
            {
                uint32_t index = i % symbols;
                value = SuperCoder::coefficient_value(symbol_id, index);

                if (!value)
                {
                    streak = true;
                    zeros++;

                    // we received the zero vector
                    if (zeros >= symbols)
                        return boost::none;
                }
                else
                {
                    if (streak) // streak ended
                    {
                        if (zeros > longest_streak)
                        {
                            pivot = boost::optional<uint32_t>(index);
                            longest_streak = zeros;
                        }

                        // no other streak can be longer
                        if (zeros >= symbols/2)
                            break;

                        streak = false;
                        zeros = 0;
                    }
                }

                ++i;
            }
            while (!value || i < symbols);

            seen_width(SuperCoder::symbols() - longest_streak);
            return pivot;
        }

        /// Finds a pivot candidate and iterates the encoding vector and
        /// subtracts existing symbols until a pivot element is found.
        /// @param symbol_data the data of the encoded symbol
        /// @param symbol_id the data constituting the encoding vector
        /// @return the pivot index if found.
        boost::optional<uint32_t> forward_substitute_to_pivot(
            value_type *symbol_data,
            value_type *symbol_id)
        {
            assert(symbol_id != 0);
            assert(symbol_data != 0);

            boost::optional<uint32_t> pivot = find_pivot(symbol_id);
            if (pivot == boost::none)
                return pivot;

            for (uint32_t j=0; j < SuperCoder::symbols(); ++j)
            {
                uint32_t i = (*pivot+j) % SuperCoder::symbols();

                value_type current_coefficient =
                    SuperCoder::coefficient_value(symbol_id, i);

                if (!current_coefficient)
                    continue;

                if (!SuperCoder::is_symbol_pivot(i))
                    return boost::optional<uint32_t>(i);

                value_type *coefficients_i =
                    SuperCoder::coefficient_vector_values(i);

                value_type *symbol_i =
                    SuperCoder::symbol_value(i);

                if (fifi::is_binary<field_type>::value)
                {
                    SuperCoder::subtract(symbol_id, coefficients_i,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::subtract(symbol_data, symbol_i,
                        SuperCoder::symbol_length());
                }
                else
                {
                    SuperCoder::multiply_subtract(
                        symbol_id, coefficients_i,
                        current_coefficient,
                        SuperCoder::coefficient_vector_length());

                    SuperCoder::multiply_subtract(symbol_data, symbol_i,
                        current_coefficient, SuperCoder::symbol_length());
                }
            }

            return boost::none;
        }

        /// Decodes a symbol based on the vector
        /// @param symbol_data buffer containing the encoding symbol
        /// @param vector_data buffer containing the encoding vector
        void decode_coefficients(
            value_type *symbol_data,
            value_type *vector_data)
        {
            assert(symbol_data != 0);
            assert(vector_data != 0);

            // Look for a pivot
            boost::optional<uint32_t> pivot_id
                = forward_substitute_to_pivot(symbol_data, vector_data);

            if (pivot_id == boost::none)
                return;

            if (!fifi::is_binary<field_type>::value)
                SuperCoder::normalize(symbol_data, vector_data, *pivot_id);

            SuperCoder::store_coded_symbol(symbol_data, vector_data, *pivot_id);
        }

        void final_decoding()
        {
            final_forward();

            if (SuperCoder::is_complete())
                final_backward_substitute();
        }

        /// Final forward substitution to put the decoding matrix on echelon form
        void final_forward()
        {
            for (uint32_t col = 0; col < SuperCoder::symbols(); ++col)
            {
                for (uint32_t i = col; i < SuperCoder::symbols(); ++i)
                {
                    value_type *vector_i =
                        SuperCoder::coefficient_vector_values(i);

                    value_type value =
                        SuperCoder::coefficient_value(vector_i, col);

                    if (value == 0)
                    {
                        // If we reached the end of the coding coefficients
                        //we did not find a pivot
                        if (i + 1 == SuperCoder::symbols())
                        {
                            value_type *vector_col =
                                SuperCoder::coefficient_vector_values( col );
                            value_type *symbol_col =
                                SuperCoder::symbol_value( col );

                            // Unless we are looking for the last pivot we
                            // check if the row below is identical if not we
                            // add it to the row below in order to conserve
                            // its information
                            if (col + 1 != SuperCoder::symbols())
                            {
                                value_type *vector_col_1 =
                                    SuperCoder::coefficient_vector_values(col+1);

                                value_type *symbol_col_1 =
                                    SuperCoder::symbol_value(col+1);

                                bool same;
                                same = std::equal(vector_col,
                                    vector_col +
                                    SuperCoder::coefficient_vector_length(),
                                    vector_col_1);

                                if (!same && SuperCoder::is_symbol_seen(col))
                                {
                                    SuperCoder::add(
                                        vector_col, vector_col_1,
                                        SuperCoder::coefficient_vector_length());

                                    SuperCoder::add(
                                        symbol_col, symbol_col_1,
                                        SuperCoder::symbol_length());
                                }
                            }
                            // Delete the row as we have no pivot for it
                            std::fill_n(vector_col,
                                SuperCoder::coefficient_vector_length(), 0);
                            SuperCoder::set_symbol_missing(col);
                        }

                        continue;
                    }

                    // If we found the pivot on a row with a higher index
                    // than the pivot we are looking for, we swap it up
                    if (i > col)
                    {
                        value_type *vector_col =
                            SuperCoder::coefficient_vector_values(col);
                        value_type *symbol_col =
                            SuperCoder::symbol_value(col);

                        value_type *vector_i =
                            SuperCoder::coefficient_vector_values(i);
                        value_type *symbol_i =
                            SuperCoder::symbol_value(i);

                        SuperCoder::add(vector_i, vector_col,
                            SuperCoder::coefficient_vector_length());

                        SuperCoder::add(symbol_i, symbol_col,
                            SuperCoder::symbol_length());

                        SuperCoder::add(vector_col, vector_i,
                            SuperCoder::coefficient_vector_length());

                        SuperCoder::add(symbol_col, symbol_i,
                            SuperCoder::symbol_length());

                        SuperCoder::add(vector_i, vector_col,
                            SuperCoder::coefficient_vector_length());

                        SuperCoder::add(symbol_i, symbol_col,
                            SuperCoder::symbol_length());
                    }

                    value_type *vector_col =
                        SuperCoder::coefficient_vector_values( col );
                    value_type *symbol_col =
                        SuperCoder::symbol_value( col );

                    if (!fifi::is_binary<field_type>::value)
                    {
                        SuperCoder::normalize(symbol_col, vector_col, col);
                    }

                    // Substitute into below rows, we use the max seen width
                    // to skip a large number of indices that must be zero
                    uint32_t start_index = std::max(
                        SuperCoder::symbols() - max_width(), i+1);

                    for (uint32_t j = start_index;
                        j < SuperCoder::symbols(); ++j)
                    {
                        value_type *vector_j =
                            SuperCoder::coefficient_vector_values(j);
                        value_type *symbol_j = SuperCoder::symbol_value(j);

                        value_type coefficient_j =
                            SuperCoder::coefficient_value(vector_j, col);

                        // If symbol exists
                        if (coefficient_j)
                        {
                            if (fifi::is_binary<field_type>::value)
                            {
                                SuperCoder::subtract(vector_j, vector_col,
                                    SuperCoder::coefficient_vector_length());

                                SuperCoder::subtract(
                                    symbol_j, symbol_col,
                                    SuperCoder::symbol_length());
                            }
                            else
                            {
                                SuperCoder::multiply_subtract(vector_j,
                                    vector_col, coefficient_j,
                                    SuperCoder::coefficient_vector_length());

                                SuperCoder::multiply_subtract(
                                    symbol_j, symbol_col,
                                    coefficient_j,
                                    SuperCoder::symbol_length());
                            }
                        }
                    }

                    //we found a pivot so no need to look further
                    break;
                }
            }
        }

        /// backwards substitute to finalize decoding
        void final_backward_substitute()
        {
            assert(SuperCoder::is_complete());

            uint32_t symbols = SuperCoder::symbols();

            for (uint32_t dest = symbols - 2; dest >= 0; --dest)
            {
                if (SuperCoder::is_symbol_decoded(dest))
                    continue;

                backward_substitute(dest);
            }
        }

        /// Backward substitute into a specified existing symbol.
        /// The perpetual decoder uses a modified version of the backward
        /// substitute to avoid inspecting the many 0 elements
        /// @param dest the index of the row to substitute into
        void backward_substitute(uint32_t dest)
        {
            uint32_t symbols = SuperCoder::symbols();
            assert(dest < symbols);

            value_type *vector_dest =
                SuperCoder::coefficient_vector_values(dest);
            value_type *symbol_dest =
                SuperCoder::symbol_value(dest);

            uint32_t highest_index = dest + max_width();
            if (highest_index >= symbols)
                highest_index = symbols;

            for (uint32_t i = highest_index - 1; i > dest; --dest)
            {
                value_type value =
                    SuperCoder::coefficient_value(vector_dest, i);

                if (value)
                {
                    value_type *symbol_i =
                        SuperCoder::symbol_value(i);
                    value_type *vector_i =
                        SuperCoder::coefficient_vector_values(i);

                    if (fifi::is_binary<field_type>::value)
                    {
                        SuperCoder::subtract(
                            vector_dest, vector_i,
                            SuperCoder::coefficient_vector_length());

                        SuperCoder::subtract(symbol_dest, symbol_i,
                            SuperCoder::symbol_length());
                    }
                    else
                    {
                        SuperCoder::multiply_subtract(
                            vector_dest, vector_i, value,
                            SuperCoder::coefficient_vector_length());

                        SuperCoder::multiply_subtract(
                            symbol_dest, symbol_i, value,
                            SuperCoder::symbol_length());
                    }
                }
            }
        }
    };
}
