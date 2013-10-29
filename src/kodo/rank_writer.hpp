// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <sak/convert_endian.hpp>

namespace kodo
{

    /// @ingroup state_api
    /// @todo docs/tests
    template<class SuperCoder>
    class rank_writer : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

        /// Helper function which writes the rank of the encoder into
        /// the payload buffer
        /// @param payload The buffer where the rank should be written
        /// @return The number of bytes written to the payload
        uint32_t write_rank(uint8_t* payload)
        {
            assert(payload != 0);

            // Write the encoder rank to the payload
            sak::big_endian::put<rank_type>(SuperCoder::rank(), payload);

            return sizeof(rank_type);
        }

    };

}

