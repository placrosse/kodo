// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup payload_codec_layers state_api
    /// @brief The payload rank recoder writes the rank of the encoder into
    ///        the payload buffer.
    ///
    /// The payload rank recoder will forward rank returned by the
    /// seen_encoder_rank() function. This is done to avoid problems where
    /// a recoder has seen a rank of e.g. 3 but itself has rank 1. If the
    /// recoder would forward its own rank another decoder might falsely
    /// detect is_partially_complete as it also sees rank 1.
    template<class SuperCoder>
    class payload_rank_writer : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

        /// The factory layer associated with this coder.
        /// In this case only needed to provide the max_payload_size()
        /// function.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_payload_size() const
            uint32_t max_payload_size() const
            {
                return SuperCoder::factory::max_payload_size() +
                    sizeof(rank_type);
            }

        };

    public:

        /// Helper function which writes the rank of the encoder into
        /// the payload buffer
        /// @param payload The buffer where the rank should be written
        /// @return The number of bytes written to the payload
        uint32_t write_rank(uint8_t* payload)
        {
            assert(payload != 0);

            // Write the encoder rank to the payload
            uint32_t written = SuperCoder::write_rank(payload);

            // If not true or payload_size() calculation will not work
            assert(written == sizeof(rank_type));

            return written;
        }

        /// @copydoc layer::payload_size() const
        uint32_t payload_size() const
        {
            return SuperCoder::payload_size() + sizeof(rank_type);
        }

    };

}

