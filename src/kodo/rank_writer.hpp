// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <sak/convert_endian.hpp>

namespace kodo
{

    /// @ingroup state_layers
    ///
    /// @brief The rank writer externalizes the current rank into the
    ///        provided buffer
    template<class SuperCoder>
    class rank_writer : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

        /// Helper function which writes the rank of the encoder into
        /// the payload buffer
        ///
        /// @note The buffer used must have at least the size reported
        ///        by the rank_info::rank_size() function.
        ///
        /// @param buffer The buffer where the rank should be written
        void write_rank(uint8_t* buffer)
        {
            assert(buffer != 0);

            // Write the encoder rank to the payload
            sak::big_endian::put<rank_type>(SuperCoder::rank(), buffer);
        }

    };

}
