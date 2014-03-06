// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

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

        /// @copydoc layer::factory
        //~ typedef typename SuperCoder::factory factory;

        /// The encoding vector
        //~ typedef typename SuperCoder::vector_type vector_type;

        /// The seed_id type
        //~ typedef uint32_t seed_id;

        /// The pivot type
        //~ typedef perpetual_base::pivot_type pivot_type;

        /// The width type
        //~ typedef perpetual_base::width_type width_type;

    protected:

        //~ using SuperCoder::m_rank;
        //~ using SuperCoder::m_coded;
        //~ using SuperCoder::m_uncoded;
        //~ uint32_t m_maximum_pivot;

        /// Keeps track of the largest seen width
        uint32_t m_max_seen_width;

    //~ public:
//~
        //~ /// The factory layer associated with this coder.
        //~ /// In this case only needed to provide the max_payload_size()
        //~ /// function.
        //~ class factory : public SuperCoder::factory
        //~ {
        //~ public:
//~
            //~ /// @see final_coder_factory::factory(...)
            //~ factory(uint32_t max_symbols, uint32_t max_symbol_size)
                //~ : SuperCoder::factory(max_symbols, max_symbol_size)
                //~ {
                //~ }
//~
            //~ /// Find the minimum buffer size needed to store a coding vector,
            //~ /// @return the minimum buffer size for a coding vector in bytes
            //~ uint32_t max_symbol_id_size() const
                //~ {
                    //~ return perpetual_base::symbol_id_size<field_type>(
                        //~ SuperCoder::factory::max_symbols());
                //~ }
        //~ };

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


        /// @see final_coder_factory(...)
        //~ void construct(uint32_t max_symbols, uint32_t max_symbol_size)
            //~ {
                //~ SuperCoder::construct(max_symbols, max_symbol_size);

                //~ uint32_t max_symbol_id_full_length =
                    //~ vector_type::length(max_symbols);
//~
                //~ m_symbol_id_full.resize(max_symbol_id_full_length);
                //~ m_symbol.resize(max_symbol_size);

                //~ m_max_seen_width = 0;
            //~ }



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
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            SuperCoder::decode_symbol(symbol_data, symbol_index);
        }


        //~ /// @copydoc layer::is_complete() const
        //~ bool is_complete() const
        //~ {
            //~ return rank() == SuperCoder::symbols();
        //~ }
//~
        //~ /// @copydoc layer::rank() const
        //~ uint32_t rank() const
        //~ {
            //~ return SuperCoder::symbols_seen() + SuperCoder::symbols_decoded();
        //~ }
//~
        //~ /// @copydoc layer::is_symbol_pivot(uint32_t) const
        //~ bool is_symbol_pivot(uint32_t index) const
        //~ {
            //~ assert(index < SuperCoder::symbols());
            //~ return SuperCoder::is_symbol_seen(index) ||
                //~ SuperCoder::is_symbol_decoded(index);
        //~ }


    protected:

        //~ uint32_t find_pivot(value_type *coefficients)
        //~ {
            //~ uint32_t max_zero_streak = 0;
            //~ uint32_t current_zero_streak = 0;
            //~ uint32_t pivot = 0;
            //~ uint32_t backside = 0;
//~
            //~ for(uint32_t i=0; i < SuperCoder::symbols() + backside; ++i)
            //~ {
                //~ if(coefficient[i])
                //~
            //~ }
//~
//~
//~
            //~ return pivot
        //~ }


        /// Decodes a symbol based on the vector
        /// @param symbol_data buffer containing the encoding symbol
        /// @param vector_data buffer containing the encoding vector
        void decode_coefficients(value_type *symbol_data,
                                 value_type *vector_data)
            {
                assert(symbol_data != 0);
                assert(vector_data != 0);

                // See if we can find a pivot
                boost::optional<uint32_t> pivot_id
                    = SuperCoder::forward_substitute_to_pivot(symbol_data, vector_data);
//~ //
                if(!pivot_id)
                {
                    return;
                    //~pivot_id = SuperCoder::forward_substitute_from_pivot(vector_data, symbol_data, pivot_id);
//~ //~
                    if(!pivot_id)
                        return;
                }

//~ //~
                if(!fifi::is_binary<field_type>::value)
                {
                    // Normalize symbol and vector
                    SuperCoder::normalize(symbol_data, vector_data, *pivot_id);
                }
                //~ std::cout << "normalized 1" <<  std::endl;
//~ //~
                // Now save the received symbol
                SuperCoder::store_coded_symbol(symbol_data, vector_data, *pivot_id);

//~ //~
                // We have increased the rank
                //~ ++m_rank;
                //~ m_coded[ *pivot_id ] = true;
//~ //~
                //~ if(*pivot_id > m_maximum_pivot)
                //~ {
                    //~ m_maximum_pivot = *pivot_id;
                //~ }
//~ //~
                if(SuperCoder::is_complete())
                {
                    final_forward();
                }
//~ //~
                if(SuperCoder::is_complete())
                {
                    final_backward_substitute();
                }

            }

        /// Final forward substitution to put the decoding matrix on echelon form
        void final_forward()
            {
                for(uint32_t col = 0; col < SuperCoder::symbols(); ++col)
                {
                    for(uint32_t i = col; i < SuperCoder::symbols(); ++i)
                    {
                        //~ value_type *vector_i = SuperCoder::vector( i );
//~
                        //~ value_type coefficient_i
                            //~ = SuperCoder::coefficient( col, vector_i );

                        value_type *vector_i =
                            SuperCoder::coefficient_vector_values(i);

                        value_type value =
                            SuperCoder::coefficient_value(vector_i, col);
                        //~ //~
                        //~ if(coefficient_i == 0)
                        if(value == 0)
                        {
                            // If we reached the end of the coding coefficients
                            //we did not find a pivot
                            if(i+1 == SuperCoder::symbols())
                            {
                                //~ value_type *vector_col = SuperCoder::vector( col );
                                //~ value_type *symbol_col = SuperCoder::symbol( col );
                                value_type *vector_col = SuperCoder::coefficient_vector_values( col );
                                value_type *symbol_col = SuperCoder::symbol_value( col );
//~ //~
                                // Unless we are looking for the last pivot we
                                // check if the row below is identical if not we
                                // add it to the row below in order to conserve
                                // its information
                                if(col+1 != SuperCoder::symbols())
                                {
                                    value_type *vector_col_1 =
                                        SuperCoder::coefficient_vector_values( col+1 );
//~ //~
                                    value_type *symbol_col_1 =
                                        SuperCoder::symbol_value( col+1 );
//~ //~
                                    bool same;
                                    same = std::equal(vector_col,
                                                      //~ vector_col + SuperCoder::vector_length(),
                                                      vector_col + SuperCoder::coefficient_vector_length(),
                                                      vector_col_1 );
//~ //~
                                    //~ if(not same && (m_coded[col] || m_uncoded[col]))
                                    if(not same && SuperCoder::is_symbol_seen(col))
                                    {
                                        SuperCoder::add(
                                            vector_col, vector_col_1,
                                            //~ SuperCoder::vector_length());
                                            SuperCoder::coefficient_vector_length());

//~ //~
                                        SuperCoder::add(
                                            symbol_col, symbol_col_1,
                                            SuperCoder::symbol_length());
                                    }
                                }
                                // Delete the row as we have no pivot for it
                                //~ std::fill_n(vector_col, SuperCoder::vector_length(), 0);
                                std::fill_n(vector_col, SuperCoder::coefficient_vector_length(), 0);
                                //~ --m_rank;
                                //~ m_uncoded[col] = false;
                                //~ m_coded[col] = false;

                                SuperCoder::set_symbol_missing(col);

                            }

//~ //~
                            continue;
                        }
//~ //~
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
                int32_t width_top = pivot_id - m_max_seen_width;
                for(uint32_t i = std::max(0, width_top); i < pivot_id; ++i)
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
                            SuperCoder::coefficient_value( vector_i, pivot_id );
//~ //~
                        if( value )
                        {
//~ //~
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
                                SuperCoder::multiply_subtract( vector_i, vector_data,
                                                              value,
                                                              SuperCoder::coefficient_vector_length());
//~ //~
                                SuperCoder::multiply_subtract( symbol_i, symbol_data,
                                                              value,
                                                              SuperCoder::symbol_length());
                            }
                        //~ }
                    }
                }
            }

    protected:

        /// Swap two rows
        /// @param i first row
        /// @param j second row
        //~ void swap(uint32_t i, uint32_t j)
            //~ {
                //~ assert(i < SuperCoder::symbols());
                //~ assert(j < SuperCoder::symbols());
//~
                //~ value_type *symbol_i = SuperCoder::symbol(i);
                //~ value_type *symbol_j = SuperCoder::symbol(j);
//~
                //~ std::copy(symbol_i,
                          //~ symbol_i + SuperCoder::symbol_length(),
                          //~ &m_symbol[0]);
//~
                //~ std::copy(symbol_j,
                          //~ symbol_j + SuperCoder::symbol_length(),
                          //~ symbol_i);
//~
                //~ std::copy(&m_symbol[0],
                          //~ &m_symbol[0]+ SuperCoder::symbol_length(),
                          //~ symbol_j);
//~
                //~ value_type *symbol_id_i = SuperCoder::vector(i);
                //~ value_type *symbol_id_j = SuperCoder::vector(j);
//~
                //~ std::copy(symbol_id_i,
                          //~ symbol_id_i + SuperCoder::vector_length(),
                          //~ &m_symbol_id_full[0]);
//~
                //~ std::copy(symbol_id_j,
                          //~ symbol_id_j + SuperCoder::vector_length(),
                          //~ symbol_id_i);
//~
                //~ std::copy(&m_symbol_id_full[0],
                          //~ &m_symbol_id_full[0]+ SuperCoder::vector_length(),
                          //~ symbol_id_j);
//~
                //~ bool temp;
                //~ temp = m_coded[i];
                //~ m_coded[i] = m_coded[j];
                //~ m_coded[j] = temp;
//~
                //~ temp = m_uncoded[i];
                //~ m_uncoded[i] = m_uncoded[j];
                //~ m_uncoded[j] = temp;
            //~ }



        /// Find the buffer size needed to store a coding vector,
        /// @return the minimum buffer size for a coding vector in bytes
        //~ uint32_t symbol_id_size() const
            //~ {
                //~ return perpetual_base::symbol_id_size<field_type>(
                    //~ SuperCoder::symbols());
            //~ }

    protected:

        /// A temporary full symbol id buffer
        //~ std::vector<value_type> m_symbol_id_full;

        /// A temporary symbol buffer
        //~ std::vector<value_type> m_symbol;
    };
}

