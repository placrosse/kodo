// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <vector>

#include <sak/storage.hpp>

namespace kodo
{

    /// @ingroup symbol_storage_layers
    /// @brief The segmented storage implementation. In this context segmented
    ///        means that the symbol storage is segmented into symbols. The
    ///        symbol storage contains the entire coding buffer internally.
    //
    /// This is useful in cases where no decoding buffer exist and
    /// swapping symbols (segments) inside the the buffer is required.
    template<class SuperCoder>
    class segmented_symbol_storage : public SuperCoder
    {
    public:

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

    public:

        /// @copydoc layer::construct(factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_data.resize(the_factory.max_symbols());

            for (uint32_t i = 0; i < the_factory.max_symbols(); ++i)
            {
                m_data[i].resize(the_factory.max_symbol_size());
            }

            m_symbols.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(uint32_t,uint32_t)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            std::fill(m_symbols.begin(), m_symbols.end(), false);
            m_symbols_count = 0;
        }

        /// @copydoc layer::symbol(uint32_t)
        uint8_t* symbol(uint32_t index)
        {
            assert(index < SuperCoder::symbols());
            return &m_data[index][0];
        }

        /// @copydoc layer::symbol(uint32_t) const
        const uint8_t* symbol(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());

            return &m_data[index][0];
        }

        /// @copydoc layer::symbol_value(uint32_t)
        value_type* symbol_value(uint32_t index)
        {
            return reinterpret_cast<value_type*>(symbol(index));
        }

        /// @copydoc layer::symbol_value(uint32_t) const
        const value_type* symbol_value(uint32_t index) const
        {
            return reinterpret_cast<const value_type*>(symbol(index));
        }

        /// @copydoc layer::swap_symbols(std::vector<std::vector<uint8_t>>&)
        void swap_symbols(std::vector< std::vector<uint8_t> >& symbols)
        {
            assert(symbols.size() == m_data.size());

            for (uint32_t i = 0; i < m_data.size(); ++i)
            {
                assert(m_data[i].size() == symbols[i].size());
            }

            m_data.swap(symbols);

            m_symbols_count = SuperCoder::symbols();
            std::fill(m_symbols.begin(), m_symbols.end(), true);
        }

        /// Swap two segments (symbols)
        /// @param first_index index of first segment (symbol) to swap
        /// @param second_index index of second segment (symbol) to swap
        void swap_segments(uint32_t first_index, uint32_t second_index)
        {
            assert(first_index != second_index);
            assert(first_index < SuperCoder::symbols());
            assert(second_index < SuperCoder::symbols());

            std::swap(m_data[first_index], m_data[second_index]);

            std::swap(m_symbols[first_index], m_symbols[second_index]);
        }

        /// @copydoc layer::set_symbols(const sak::const_storage&)
        void set_symbols(const sak::const_storage& symbol_storage)
        {
            auto symbol_sequence = sak::split_storage(
                symbol_storage, SuperCoder::symbol_size());

            uint32_t sequence_size = symbol_sequence.size();
            assert(sequence_size == SuperCoder::symbols());

            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                set_symbol(i, symbol_sequence[i]);
            }
        }

        /// @copydoc layer::set_symbol(uint32_t, const sak::const_storage&)
        void set_symbol(uint32_t index, const sak::const_storage& symbol)
        {
            assert(symbol.m_data != 0);
            assert(symbol.m_size == SuperCoder::symbol_size());
            assert(index < SuperCoder::symbols());

            sak::mutable_storage dest_data = sak::storage(m_data[index]);

            sak::copy_storage(dest_data, symbol);

            if (m_symbols[index] == false)
            {
                ++m_symbols_count;
                m_symbols[index] = true;
            }
        }

        /// @copydoc layer::copy_symbols(const sak::mutable_storage&)
        void copy_symbols(const sak::mutable_storage& dest) const
        {
            auto storage = dest;

            assert(storage.m_size > 0);
            assert(storage.m_data != 0);

            uint32_t data_to_copy =
                std::min(storage.m_size, SuperCoder::block_size());

            uint32_t symbol_index = 0;

            while(data_to_copy > 0)
            {
                uint32_t copy_size =
                    std::min(data_to_copy, SuperCoder::symbol_size());

                data_to_copy -= copy_size;

                sak::const_storage src_storage =
                    sak::storage(symbol(symbol_index), copy_size);

                sak::copy_storage(storage, src_storage);

                storage.m_size -= copy_size;
                storage.m_data += copy_size;

                ++symbol_index;
            }
        }

        /// @copydoc layer::copy_symbol(
        ///              uint32_t, const sak::mutable_storage&) const
        void copy_symbol(uint32_t index,
                         const sak::mutable_storage& dest) const
        {
            auto storage = dest;

            assert(storage.m_size > 0);
            assert(storage.m_data != 0);

            uint32_t data_to_copy =
                std::min(storage.m_size, SuperCoder::symbol_size());

            sak::const_storage src =
                sak::storage(symbol(index), data_to_copy);

            sak::copy_storage(storage, src);
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

    protected:

        /// Stores the symbol data
        std::vector< std::vector<uint8_t> > m_data;

        /// Symbols count
        uint32_t m_symbols_count;

        /// Tracks which symbols have been set
        std::vector<bool> m_symbols;
    };

}


