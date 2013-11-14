// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <algorithm>
#include <sak/convert_endian.hpp>

namespace kodo
{

    /// @ingroup state_reader
    /// @brief The payload decoder splits the payload buffer into
    ///        symbol header and symbol.
    template<class SuperCoder>
    class rank_reader : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            // Reset the state
            m_remote_rank = 0;
        }

        /// Reads the rank of the encoder from the payload buffer
        /// @param payload The payload buffer
        /// @return The amount of bytes read
        uint32_t read_rank(uint8_t* payload)
        {
            assert(payload != 0);

            // Write the encoder rank to the payload
            rank_type remote_rank =
                sak::big_endian::get<rank_type>(payload);

            // We set the maximum rank of the previously seen and the
            // newly received. Re-ordering of packets might cause a
            // the remote rank to be lower than what has previously
            // been seen
            m_remote_rank = std::max(remote_rank, m_remote_rank);

            return sizeof(rank_type);
        }

        /// @return The rank of the encoder as read from the packet
        rank_type remote_rank() const
        {
            return m_remote_rank;
        }

    private:

        /// Stores the read remote rank
        rank_type m_remote_rank;

    };

}


