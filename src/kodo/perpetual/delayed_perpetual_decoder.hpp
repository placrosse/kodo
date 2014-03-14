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

    /// Implementes a simple uniform random encoding scheme
    /// where the payload_id carries all coding coefficients
    /// i.e. the "encoding vector"
    template<class SuperCoder>
    class delayed_perpetual_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    protected:

        uint32_t m_max_seen_width;

    public:

        /// Constructor
        delayed_perpetual_decoder()
            : m_max_seen_width(SuperCoder::symbols()-1)
        { }
            ///@Todo start width zero width


        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            //@TODO initialize to zero and update when symbols arrive
            m_max_seen_width = SuperCoder::symbols()-1;
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            SuperCoder::decode_symbol(symbol_data, symbol_index);

            if(SuperCoder::is_complete())
                final_decoding();
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            assert(symbol_data != 0);
            assert(coefficients != 0);

            value_type *s =
                reinterpret_cast<value_type*>(symbol_data);

            value_type *c =
                reinterpret_cast<value_type*>(coefficients);

            decode_coefficients(s, c);

            if(SuperCoder::is_complete())
                final_decoding();
        }

    protected:

        struct sort
        {
            // sort highest zero count first, then highest index
            static bool zeros(const std::pair<uint32_t, uint32_t> &i,
                              const std::pair<uint32_t, uint32_t> &j)
            {
                if( i.first < j.first ) return false;
                if( j.first < i.first ) return true;
                return j.second < i.second;
            }

            // sort on first index first
            static bool index(const std::pair<uint32_t, uint32_t> &i,
                               const std::pair<uint32_t, uint32_t> &j)
            {
                if( i.second > j.second ) return false;
                else return true; // indices are unique
            }
        };

        /// Find the intended pivot index in a perpetual coding vector, which is
        /// the index following the longest streak of zeros in the vector
        /// @param symbol_id the coding vector
        /// @return a pivot candidate if found
        boost::optional<uint32_t> find_pivot(value_type *symbol_id)
        {
            // Look for all zero streaks in the coding vector
            uint32_t symbols = SuperCoder::symbols();
            bool streak = false;
            uint32_t zeros = 0;
            std::vector<std::pair<uint32_t, uint32_t>> zero_streaks;

            //~ for(uint32_t i=0; i < symbols; ++i)
            uint32_t i=0;
            while(i < symbols)
            {
                value_type current_coefficient =
                    SuperCoder::coefficient_value(symbol_id, i);

                if(!current_coefficient)
                {
                    if(streak) //continue zero streak
                    {
                        zeros ++;
                    }
                    else //new zero streak
                    {
                        zeros = 1;
                        streak = true;
                    }
                }
                else
                {
                    if(streak) //end of zero streak
                    {
                        std::pair<uint32_t, uint32_t> zero_streak(zeros,i);
                        zero_streaks.push_back(zero_streak);
                        streak = false;
                    }
                }
                i++;
            }

            if(streak) // if the last value is zero
            {
                std::pair<uint32_t, uint32_t> zero_streak(zeros,i);
                zero_streaks.push_back(zero_streak);
            }

            // All values in the vector was non-zero
            if(zero_streaks.size() == 0)
                return boost::optional<uint32_t>(0);

            std::sort(zero_streaks.begin(), zero_streaks.end(), sort::zeros);

            // if the longest streak does not terminate at the end
            if (zero_streaks[0].second != symbols-1)
                return zero_streaks[0].second;

            std::sort(zero_streaks.begin(), zero_streaks.end(), sort::index);

            //in case the first value in the symbol id is non-zero
            if(zero_streaks[0].second - zero_streaks[0].first)
                return boost::optional<uint32_t>(0);

            return zero_streaks[0].second;
        }

        boost::optional<uint32_t> forward_substitute_to_pivot(
            value_type *symbol_data,
            value_type *symbol_id)
        {
            assert(symbol_id != 0);
            assert(symbol_data != 0);

            boost::optional<uint32_t> pivot = find_pivot(symbol_id);
            if(pivot == boost::none)
                return pivot;

            for(uint32_t j=0; j < SuperCoder::symbols(); ++j)
            {
                uint32_t i = (*pivot+j) % SuperCoder::symbols();

                value_type current_coefficient =
                    SuperCoder::coefficient_value(symbol_id, i);

                if(!current_coefficient)
                    continue;

                if(!SuperCoder::is_symbol_pivot(i))
                    return boost::optional<uint32_t>(i);

                value_type *coefficients_i =
                    SuperCoder::coefficient_vector_values(i);

                value_type *symbol_i =
                    SuperCoder::symbol_value(i);

                if(fifi::is_binary<field_type>::value)
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
        void decode_coefficients(value_type *symbol_data,
                                 value_type *vector_data)
            {
                assert(symbol_data != 0);
                assert(vector_data != 0);

                // See if we can find a pivot
                // @TODO find the pivot after the longest streak of zeros and not just the first one
                boost::optional<uint32_t> pivot_id
                    = forward_substitute_to_pivot(symbol_data, vector_data);

                if(pivot_id == boost::none)
                    return;

                if(!fifi::is_binary<field_type>::value)
                {
                    // Normalize symbol and vector
                    SuperCoder::normalize(symbol_data, vector_data, *pivot_id);
                }

                // Save the received symbol
                SuperCoder::store_coded_symbol(symbol_data, vector_data, *pivot_id);
            }

        void final_decoding()
        {
            final_forward();

            //~ std::cout << "final backwards" << std::endl;
            if(SuperCoder::is_complete())
                final_backward_substitute();

        }

        /// Final forward substitution to put the decoding matrix on echelon form
        void final_forward()
            {
                for(uint32_t col = 0; col < SuperCoder::symbols(); ++col)
                {
                    for(uint32_t i = col; i < SuperCoder::symbols(); ++i)
                    {
                        value_type *vector_i =
                            SuperCoder::coefficient_vector_values(i);

                        value_type value =
                            SuperCoder::coefficient_value(vector_i, col);

                        if(value == 0)
                        {
                            // If we reached the end of the coding coefficients
                            //we did not find a pivot
                            if(i+1 == SuperCoder::symbols())
                            {
                                value_type *vector_col = SuperCoder::coefficient_vector_values( col );
                                value_type *symbol_col = SuperCoder::symbol_value( col );

                                // Unless we are looking for the last pivot we
                                // check if the row below is identical if not we
                                // add it to the row below in order to conserve
                                // its information
                                if(col+1 != SuperCoder::symbols())
                                {
                                    value_type *vector_col_1 =
                                        SuperCoder::coefficient_vector_values( col+1 );

                                    value_type *symbol_col_1 =
                                        SuperCoder::symbol_value( col+1 );

                                    bool same;
                                    same = std::equal(vector_col,

                                                      vector_col + SuperCoder::coefficient_vector_length(),
                                                      vector_col_1 );

                                    if(not same && SuperCoder::is_symbol_seen(col))
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
                                std::fill_n(vector_col, SuperCoder::coefficient_vector_length(), 0);
                                SuperCoder::set_symbol_missing(col);
                            }

                            continue;
                        }

                        // A pivot was found
                        // If we found the pivot on a row with a higher
                        // index than the pivot we are looking for, we swap it up
                        if(i > col)
                        {
                            value_type *vector_col = SuperCoder::coefficient_vector_values(col);
                            value_type *symbol_col = SuperCoder::symbol_value(col);

                            value_type *vector_i = SuperCoder::coefficient_vector_values(i);
                            value_type *symbol_i = SuperCoder::symbol_value(i);

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

                        //~ value_type *vector_col = SuperCoder::vector( col );
                        //~ value_type *symbol_col = SuperCoder::symbol( col );
                        value_type *vector_col = SuperCoder::coefficient_vector_values( col );
                        value_type *symbol_col = SuperCoder::symbol_value( col );

                        if(!fifi::is_binary<field_type>::value)
                        {
                            SuperCoder::normalize(symbol_col, vector_col, col);
                        }
//~ //~
                        // Substitute into below rows, we use the max seen width
                        // to skip a large number of indices that must be zero
                        uint32_t start_index = std::max(
                            SuperCoder::symbols() - m_max_seen_width, i+1);
//~ //~
                        for(uint32_t j = start_index; j < SuperCoder::symbols(); ++j)
                        {
                            //~ value_type *vector_j = SuperCoder::vector( j );
                            //~ value_type *symbol_j = SuperCoder::symbol( j );
                            value_type *vector_j = SuperCoder::coefficient_vector_values(j);
                            value_type *symbol_j = SuperCoder::symbol_value(j);
//~ //~
                            //~ value_type coefficient_j
                                //~ = SuperCoder::coefficient( col, vector_j );
                            value_type coefficient_j
                                = SuperCoder::coefficient_value(vector_j, col);

//~ //~
                            // If symbol exists
                            if( coefficient_j)
                            {
                                if(fifi::is_binary<field_type>::value)
                                {
                                    SuperCoder::subtract(
                                        vector_j, vector_col,
                                        //~ SuperCoder::vector_length());
                                        SuperCoder::coefficient_vector_length());
//~ //~
                                    SuperCoder::subtract(
                                        symbol_j, symbol_col,
                                        SuperCoder::symbol_length());
                                }
                                else
                                {
                                    SuperCoder::multiply_subtract(
                                        vector_j, vector_col,
                                        coefficient_j,
                                        //~ SuperCoder::vector_length());
                                        SuperCoder::coefficient_vector_length());
//~ //~
                                    SuperCoder::multiply_subtract(
                                        symbol_j, symbol_col,
                                        coefficient_j,
                                        SuperCoder::symbol_length());
                                }
                            }
                        }
//~ //~
                        //we found a pivot so no need to look further
                        break;
                    }
                }
            }


        //~ /// Final backwards substitution
        void final_backward_substitute()
        {
            assert(SuperCoder::is_complete());
//~ //~
            uint32_t symbols = SuperCoder::symbols();
//~ //~
            for(uint32_t i = symbols; i --> 0;)
            {
                //~ value_type *vector_i = SuperCoder::vector(i);
                //~ value_type *symbol_i = SuperCoder::symbol(i);
                value_type *vector_i = SuperCoder::coefficient_vector_values(i);
                value_type *symbol_i = SuperCoder::symbol_value(i);

                backward_substitute(symbol_i, vector_i, i);
            }
        }
//~
//~
//~
        /// Backward substitute the found symbol into the
        /// existing symbols.
        /// The perpetual decoder uses a slightly modified version of the
        /// bacwards_substitute from linear block decoder, to avoid inspecting
        ///the many 0 elements
        /// @param pivot_id the pivot index of the symbol in the
        ///        buffers vector_data and symbol_data
        /// @param vector_data buffer containing the encoding vector
        /// @param symbol_data buffer containing the encoding symbol
        void backward_substitute(const value_type *symbol_data,
                                 const value_type *vector_data,
                                 uint32_t pivot_id)
        {
            assert(vector_data != 0);
            assert(symbol_data != 0);
//~ //~
            assert(pivot_id < SuperCoder::symbols());
//~ //~
            // These asserts can go away since the function
            // will also work for packets already received (mvp).
//                assert(m_uncoded[pivot_id] == false);
//                assert(m_coded[pivot_id] == false);
//~ //~
            // We found a "1" that nobody else had as pivot, we now
            // substract this packet from other coded packets
            // - if they have a "1" on our pivot place


            uint32_t width_top = 0;
            if(m_max_seen_width < pivot_id)
                width_top = pivot_id - m_max_seen_width;

            for(uint32_t i = width_top; i < pivot_id; ++i)
            {
                //~ if( m_uncoded[i] )
                if( SuperCoder::is_symbol_decoded(i) )
                {
                    // We know that we have no non-zero elements
                    // outside the pivot position.
                    continue;
                }
//~ //~
                // if(i == pivot_id)
                // {
                //     // We cannot backward substitute into ourself
                //     continue;
                // }
//~ //~
                //~ if( m_coded[i] )
                //~ {

                //~ value_type *vector_i = SuperCoder::vector(i);
                value_type *vector_i = SuperCoder::coefficient_vector_values(i);
//~ //~
                value_type value =
                    SuperCoder::coefficient_value(vector_i, pivot_id);
//~ //~
                if(value)
                {
                    //~ value_type *symbol_i = SuperCoder::symbol(i);
                    value_type *symbol_i = SuperCoder::symbol_value(i);
//~ //~
                    if(fifi::is_binary<field_type>::value)
                    {
                        SuperCoder::subtract(vector_i, vector_data,
                                             SuperCoder::coefficient_vector_length());
//~ //~
                        SuperCoder::subtract(symbol_i, symbol_data,
                                             SuperCoder::symbol_length());
                    }
                    else
                    {
//~ //~
                        // Update symbol and corresponding vector
                        SuperCoder::multiply_subtract(vector_i, vector_data,
                                                      value,
                                                      SuperCoder::coefficient_vector_length());
//~ //~
                        SuperCoder::multiply_subtract(symbol_i, symbol_data,
                                                      value,
                                                      SuperCoder::symbol_length());
                    }
                }
            }
        }

    };
}

