// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <vector>

#include <sak/storage.hpp>

namespace kodo
{
    /// @todo
    template<class SuperCoder>
    class status_shallow_symbol_storage : public SuperCoder
    {
    public:

        /// The storage type used
        using storage_type = typename SuperCoder::storage_type;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);

            m_data.resize(the_factory.max_symbols(), 0);
        }

        /// @copydoc layer::initialize(uint32_t,uint32_t)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            SuperCoder::initialize(the_factory);

            std::fill(m_data.begin(), m_data.end(), (data_ptr) 0);
            m_symbols_count = 0;
        }

        /// @copydoc layer::symbol(uint32_t) const
        const uint8_t* symbol(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());

            // Did you forget to set the symbol
            assert(m_data[index]);

            return m_data[index];
        }

        /// @copydoc layer::symbol_value(uint32_t) const
        const value_type* symbol_value(uint32_t index) const
        {
            return reinterpret_cast<const value_type*>(symbol(index));
        }

        /// @copydoc layer::swap_symbols(std::vector<data_ptr>&)
        void swap_symbols(std::vector<data_ptr> &symbols)
        {
            assert(m_data.size() == symbols.size());
            m_data.swap(symbols);

            m_symbols_count = 0;

            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                if(m_data[i] != 0)
                    ++m_symbols_count;
            }
        }

        /// @copydoc layer::set_symbols()
        void set_symbols(const storage_type &symbol_storage)
        {
            auto symbol_sequence = sak::split_storage(
                symbol_storage, SuperCoder::symbol_size());

            auto sequence_size = symbol_sequence.size();
            assert(sequence_size == SuperCoder::symbols());

            for(uint32_t i = 0; i < sequence_size; ++i)
            {
                set_symbol(i, symbol_sequence[i]);
            }
        }

        /// @copydoc layer::set_symbol()
        void set_symbol(uint32_t index, const storage_type &symbol)
        {
            assert(index < SuperCoder::symbols());

            SuperCoder::set_symbol(index, symbol);

            if(m_symbols[index] == false)
            {
                ++m_symbols_count;
                m_symbols[index] = true;
            }

            // Assign the pointer
            m_data[index] = symbol.m_data;
        }

        /// @copydoc layer::symbols_available() const
        uint32_t symbols_available() const
        {
            return m_symbols_count;
        }

        /// @copydoc layer::symbols_initialized() const
        uint32_t symbols_initialized() const
        {
            return symbols_available();
        }

        /// @copydoc layer::is_symbols_available() const
        bool is_symbols_available() const
        {
            return m_symbols_count == SuperCoder::symbols();
        }

        /// @copydoc layer::is_symbols_initialized() const
        bool is_symbols_initialized() const
        {
            return is_symbols_available();
        }

        /// @copydoc layer::is_symbol_available(uint32_t) const
        bool is_symbol_available(uint32_t symbol_index) const
        {
            return m_data[symbol_index] != 0;
        }

        /// @copydoc layer::is_symbol_initialized(uint32_t) const
        bool is_symbol_initialized(uint32_t symbol_index) const
        {
            return is_symbol_available(symbol_index);
        }

    protected:

        /// Symbols count
        uint32_t m_symbols_count;

    };
}
