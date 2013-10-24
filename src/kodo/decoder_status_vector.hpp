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
    /// @brief The symbol decoding status tracker stores information about
    ///        the status of the different symbols contained within a decoder.
    template<class SuperCoder>
    class decoder_status_vector : public SuperCoder
    {
    public:

        /// The data used for the status vector
        typedef uint8_t status_block_type;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_symbols_seen.resize(the_factory.max_symbols(), false);
            m_symbols_decoded.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_symbols_seen.resize(the_factory.symbols(), false);
            m_symbols_decoded.resize(the_factory.symbols(), false);
        }

        /// @copydoc layer::set_symbol_missing(uint32_t)
        void set_symbol_missing(uint32_t index)
        {
            assert(index < m_symbols_seen.size());
            m_symbols_seen[index] = false;
            m_symbols_decoded[index] = false;
        }

        /// @copydoc layer::set_symbol_seen(uint32_t)
        void set_symbol_seen(uint32_t index)
        {
            assert(index < m_symbols_seen.size());
            m_symbols_seen[index] = true;
            m_symbols_decoded[index] = false;
        }

        /// @copydoc layer::set_symbol_decoded(uint32_t)
        void set_symbol_decoded(uint32_t index)
        {
            assert(index < m_symbols_decoded.size());
            m_symbols_decoded[index] = true;
            m_symbols_seen[index] = false;
        }

        /// @copydoc layer::is_symbol_missing(uint32_t) const
        bool is_symbol_missing(uint32_t index) const
        {
            assert(index < m_symbols_seen.size());
            return !m_symbols_seen[index] && !m_symbols_decoded[index];
        }

        /// @copydoc layer::is_symbol_seen(uint32_t) const
        bool is_symbol_seen(uint32_t index) const
        {
            assert(index < m_symbols_seen.size());
            return m_symbols_seen[index];
        }

        /// @copydoc layer::is_symbol_decoded(uint32_t) const
        bool is_symbol_decoded(uint32_t index) const
        {
            assert(index < m_symbols_decoded.size());
            return m_symbols_decoded[index];
        }

        /// @copydoc layer::symbols_seen() const
        uint32_t symbols_seen() const
        {
            return m_symbols_seen.count();
        }

        /// @copydoc layer::symbols_decoded() const
        uint32_t symbols_decoded() const
        {
            return m_symbols_decoded.count();
        }

        /// @copydoc layer::symbols_missing() const
        uint32_t symbols_missing() const
        {
            return SuperCoder::symbols() - symbols_decoded() - symbols_seen();
        }

        /// Writes the decoder status to the provided buffer. The decoder status
        /// is a bit vector where each symbol represented by a 0 if the symbol
        /// is missing or a 1 if the symbol is seen or decoded.
        void write_decoder_rank_status(uint8_t *buffer) const
        {
            assert(buffer);
            boost::to_block_range(m_symbols_seen | m_symbols_decoded, buffer);
        }

        /// @return The size in bytes of decoder status vector
        uint32_t decoder_rank_status_size() const
        {
            assert(m_symbols_seen.num_blocks() ==
                   m_symbols_decoded.num_blocks());

            static_assert(sizeof(status_block_type) == 1,
                          "We assume that the block type is 1 byte in the "
                          "calculation here");

            return m_symbols_seen.num_blocks();
        }

    private:

        /// Tracks the symbols which have been "seen", see the
        /// symbol_decoding_status_tracker layer for a description of
        /// what seen means.
        boost::dynamic_bitset<status_block_type> m_symbols_seen;

        /// Tracks the symbols which have been fully decoded.
        boost::dynamic_bitset<status_block_type> m_symbols_decoded;
    };

}
