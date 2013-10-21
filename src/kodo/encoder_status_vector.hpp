// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <boost/dynamic_bitset.hpp>

namespace kodo
{

    /// @ingroup decoder_api
    /// @brief The encoder status vector stores information about
    ///        the seen and/or decoded symbols at the decoder side.
    ///        The user must provide this information by calling
    ///        decoder_status_vector::get_decoder_status(uint8_t *)
    ///        on the decoder, and then parse the resulting buffer
    ///        to encoder_status_vector::set_decoder_status() on the
    ///        encoder side.
    template<class SuperCoder>
    class encoder_status_vector : public SuperCoder
    {

        typedef uint8_t status_block;
        typedef boost::dynamic_bitset<status_block> status_container;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);

            m_decoder_symbols.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_decoder_symbols.reset();
        }

        bool decoder_has_symbol(uint32_t index) const
        {
            return m_decoder_symbols[index];
        }

        size_t decoder_symbols() const
        {
            return m_decoder_symbols.count();
        }

        void set_decoder_status(uint8_t *first)
        {
            assert(first != NULL);

            status_block *last = first + decoder_status_len();
            boost::from_block_range(first, last, m_decoder_symbols);
        }

        size_t decoder_status_len() const
        {
            return m_decoder_symbols.num_blocks();
        }

        const status_container &decoder_status() const
        {
            return m_decoder_symbols;
        }

    private:

        // bitmap for symbols seen or decoded by the decoder
        status_container m_decoder_symbols;

    };

}
