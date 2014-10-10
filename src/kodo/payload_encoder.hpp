// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @ingroup payload_codec_layers
    ///
    /// @brief The payload encoder splits the payload buffer into
    ///        symbol header and symbol.
    template<class SuperCoder>
    class payload_encoder : public SuperCoder
    {
    public:

        /// @ingroup factory_layers
        ///
        /// The factory layer associated with this coder.
        /// In this case only needed to provide the max_payload_size()
        /// function.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory_base::max_payload_size() const
            uint32_t max_payload_size() const
            {
                return SuperCoder::factory_base::max_symbol_size() +
                    SuperCoder::factory_base::max_header_size();
            }
        };

    public:

        /// Encodes a symbol to the provided buffer using the following
        /// layout:
        ///
        /// @code
        ///   +-------------------+---------------+
        ///   |    symbol data    |   symbol id   |
        ///   +-------------------+---------------+
        /// @endcode
        ///
        /// The reason the symbol data is placed first in the payload
        /// buffer is to increase the chances of 16 byte-alignment of
        /// the symbol data.
        /// If the variable length id would be place in front of the
        /// symbol it would easily become unaligned. Unaligned symbol
        /// data access will most likely result in very bad performance.
        ///
        /// @copydoc layer::encode(uint8_t*)
        uint32_t encode(uint8_t *payload)
        {
            assert(payload != 0);

            uint8_t *symbol_data = payload;
            uint8_t *symbol_id = payload + SuperCoder::symbol_size();

            // The non-payload layers only return the number of bytes
            // use for the symbol_id here we return _all_ the bytes used.
            return SuperCoder::encode(symbol_data, symbol_id)
                + SuperCoder::symbol_size();
        }

        /// @copydoc layer::payload_size() const
        uint32_t payload_size() const
        {
            return SuperCoder::symbol_size() +
                SuperCoder::header_size();
        }
    };
}
