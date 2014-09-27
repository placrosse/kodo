// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <sak/convert_endian.hpp>

#include "carousel_common.hpp"

namespace kodo
{

    /// @ingroup codec_header_layers
    /// @brief Carousel decoder takes symbols produced by a carousel encoder
    ///        and decodes them.
    template<class SuperCoder>
    class carousel_decoder : public SuperCoder
    {
    public:

        /// The symbol id
        typedef carousel_common::id_type id_type;

    public:

        /// The factory_base layer associated with this coder.
        /// In this case only needed to provide the max_payload_size()
        /// function.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::max_header_size() const
            uint32_t max_header_size() const
            {
                return sizeof(id_type);
            }
        };

    public:

        /// @copydoc layer::decode(uint8_t*,uint8_t*)
        void decode(uint8_t *symbol_data, uint8_t *symbol_header)
        {
            assert(symbol_data != 0);
            assert(symbol_header != 0);

            id_type id = sak::big_endian::get<id_type>(symbol_header);

            assert(id < SuperCoder::symbols());

            SuperCoder::decode_symbol(symbol_data, id);
        }

        /// @copydoc layer::header_size() const
        uint32_t header_size() const
        {
            return sizeof(id_type);
        }

    };

}
