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

    /// @todo Read doc
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
    public:

        /// The data used for the status vector
        typedef uint8_t status_block_type;

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

            m_decoder_symbols.resize(the_factory.symbols(), false);
        }

        /// Queries the decoding status vector to check whether a specific
        /// symbols has been "seen" or fully decoded by the decoder
        /// @param index The index of the symbol whos state which should be
        ///        checked
        /// @return True if the symbol has been "seen" or fully decoded and
        ///         False if the symbol is missing at the decoder.
        bool decoder_has_symbol(uint32_t index) const
        {
            return m_decoder_symbols[index];
        }

        /// @return The number of symbols that the decoder has either "seen"
        ///         or fully decoded.
        uint32_t decoder_rank() const
        {
            return m_decoder_symbols.count();
        }

        /// Reads the decoder rank status. This function expects as
        /// input a bit vector where the index zero bit denotes the
        /// status of the symbol with index zero. If the bit is 0 it
        /// means that the symbol is still missing at the
        /// decoder. Otherwise if the bit is 1 it means that the
        /// symbol has been either seen or fully decoded by the
        /// decoder.
        /// @input buffer The input buffer containing the bit vector
        void read_decoder_rank_status(const uint8_t *buffer)
        {
            assert(buffer);

            const uint8_t* last = buffer + decoder_status_size();
            boost::from_block_range(buffer, last, m_decoder_symbols);
        }

        /// @return The size in bytes of decoder status vector
        uint32_t decoder_rank_status_size() const
        {
            static_assert(sizeof(status_block_type) == 1,
                          "We assume that the block type is 1 byte in the "
                          "calculation here");

            return m_decoder_symbols.num_blocks();
        }

        /// @todo is this one needed
        // const status_container& decoder_status() const
        // {
        //     return m_decoder_symbols;
        // }

    private:

        /// Tracks the symbols which have been seen or fully decoded
        /// by the decoder.
        boost::dynamic_bitset<status_block_type> m_decoder_symbols;

    };

}
