// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <sak/convert_endian.hpp>

#include "rank_reader.hpp"

namespace kodo
{

    /// @ingroup payload_codec_layers
    /// @brief The payload decoder splits the payload buffer into
    ///        symbol header and symbol.
    template<class SuperCoder>
    class payload_rank_decoder : public rank_reader<SuperCoder>
    {
    public:

        /// The actual super type
        typedef rank_reader<SuperCoder> Super;

        /// @copydoc layer::rank_type
        typedef typename Super::rank_type rank_type;

    public:

        /// The factory layer associated with this coder.
        /// In this case only needed to provide the max_payload_size()
        /// function.
        class factory : public Super::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : Super::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_payload_size() const
            uint32_t max_payload_size() const
            {
                return Super::factory::max_payload_size() +
                    sizeof(rank_type);
            }
        };

    public:

        /// Unpacks the symbol data and symbol header from the payload
        /// buffer.
        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t* payload)
        {
            assert(payload != 0);

            uint32_t read = Super::read_rank(payload);

            // If not true or payload_size() calculation will not work
            assert(read == sizeof(rank_type));

            Super::decode(payload + read);
        }

        /// @copydoc layer::payload_size() const
        uint32_t payload_size() const
        {
            return SuperCoder::payload_size() +
                sizeof(rank_type);
        }

    };
}


