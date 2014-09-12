// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <vector>

#include <fifi/fifi_utils.hpp>
#include <sak/storage.hpp>

namespace kodo
{
    /// @ingroup symbol_storage_layers
    ///
    /// @brief The deep storage implementation. In this context deep
    ///        means that the symbol storage contains the entire coding
    ///        buffer internally.
    ///
    /// This is useful in cases where incoming data is to be
    /// decoded and no existing decoding buffer exist.
    template<class SuperCoder>
    class status_deep_symbol_storage : public SuperCoder
    {
    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);
            m_symbols.resize(the_factory.max_symbols());
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            std::fill(m_symbols.begin(), m_symbols.end(), false);

            m_symbols_count = 0;
        }

        /// @copydoc layer::set_symbols(const sak::const_storage&)
        void set_symbols(const sak::const_storage &symbol_storage)
        {
            SuperCoder::set_symbols(symbol_storage);

            // This will specify all symbols, also in the case
            // of partial data. If this is not desired then the
            // symbols need to be set individually.
            m_symbols_count = SuperCoder::symbols();
            std::fill(m_symbols.begin(), m_symbols.end(), true);
        }

        /// @copydoc layer::set_symbol(uint32_t, const sak::const_storage&)
        void set_symbol(uint32_t index, const sak::const_storage &symbol)
        {
            assert(index < SuperCoder::symbols());

            SuperCoder::set_symbol(index, symbol);

            if(m_symbols[index] == false)
            {
                ++m_symbols_count;
                m_symbols[index] = true;
            }
        }

        /// @copydoc layer::swap_symbols(std::vector<uint8_t> &)
        void swap_symbols(std::vector<uint8_t> &symbols)
        {
            SuperCoder::swap_symbols(symbols);
            m_symbols_count = SuperCoder::symbols();
            std::fill(m_symbols.begin(), m_symbols.end(), true);
        }

        /// @copydoc layer::symbols_available() const
        uint32_t symbols_available() const
        {
            return SuperCoder::symbols();
        }

        /// @copydoc layer::symbols_initialized() const
        uint32_t symbols_initialized() const
        {
            return m_symbols_count;
        }

        /// @copydoc layer::is_symbols_available() const
        bool is_symbols_available() const
        {
            return true;
        }

        /// @copydoc layer::is_symbols_initialized() const
        bool is_symbols_initialized() const
        {
            return m_symbols_count == SuperCoder::symbols();
        }

        /// @copydoc layer::is_symbol_available(uint32_t) const
        bool is_symbol_available(uint32_t /*symbol_index*/) const
        {
            return true;
        }

        /// @copydoc layer::is_symbol_initialized(uint32_t) const
        bool is_symbol_initialized(uint32_t symbol_index) const
        {
            return m_symbols[symbol_index];
        }

    private:

        /// Symbols count
        uint32_t m_symbols_count;

        /// Tracks which symbols have been set
        std::vector<bool> m_symbols;
    };
}
