// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef KODO_DEBUG_RLNC_FULL_VECTOR_DECODER_DEBUG_HPP
#define KODO_DEBUG_RLNC_FULL_VECTOR_DECODER_DEBUG_HPP

#include <stdint.h>

namespace kodo
{
    // Debug layer which allows printing encoding vectors
    // of a RLNC decoder.
    template<class SuperCoder>
    class full_vector_decoder_debug : public SuperCoder
    {
    public:

        // The field we use
        typedef typename SuperCoder::field_type field_type;

        // The value_type used to store the field elements
        typedef typename field_type::value_type value_type;

        // The encoding vector
        typedef linear_block_vector<field_type> vector_type;

        // Access to different variables
        using SuperCoder::m_uncoded;
        using SuperCoder::m_coded;

    public:

        // @param out, the output stream
        void print_encoding_vectors(std::ostream &out)
            {
                for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
                {
                    if( m_uncoded[i] )
                    {
                        out << i << " U:\t";
                    }
                    else if( m_coded[i] )
                    {
                        out << i << " C:\t";
                    }
                    else
                    {
                        out << i << " ?:\t";
                    }

                    value_type *vector_i = SuperCoder::vector(i);

                    for(uint32_t j = 0; j < SuperCoder::symbols(); ++j)
                    {

                        value_type value = vector_type::coefficient( j, vector_i );
                        std::cout << (uint32_t)value << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
    };
}

#endif
