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
    class symbol_decoding_status_bitset : public SuperCoder
    {

        typedef uint8_t status_block;
        typedef boost::dynamic_bitset<status_block> status_container;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
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

            m_symbols_seen.reset();
            m_symbols_decoded.reset();
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

        size_t symbols_seen() const
        {
            return m_symbols_seen.count();
        }

        size_t symbols_decoded() const
        {
            return m_symbols_decoded.count();
        }

        size_t symbols_missing() const
        {
            return SuperCoder::symbols() - symbols_decoded() - symbols_seen();
        }

        void get_symbols_seen(uint8_t *data) const
        {
            get_symbols(m_symbols_seen, data);
        }

        void set_symbols_seen(uint8_t *data)
        {
            set_symbols(m_symbols_seen, data);
        }

        void get_symbols_decoded(uint8_t *data) const
        {
            get_symbols(m_symbols_decoded, data);
        }

        void set_symbols_decoded(uint8_t *data)
        {
            set_symbols(m_symbols_decoded, data);
        }

        size_t symbols_status_len() const
        {
            return m_symbols_seen.num_blocks();
        }

    private:

        void get_symbols(const status_container &symbols, uint8_t *first) const
        {
            assert(first != NULL);
            boost::to_block_range(symbols, first);
        }

        void set_symbols(status_container &symbols, uint8_t *first)
        {
            assert(first != NULL);
            status_block *last = first + symbols_status_len();
            symbols = status_container(first, last);
        }

        status_container m_symbols_seen;
        status_container m_symbols_decoded;
    };

}
