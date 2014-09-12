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
    class deep_symbol_storage : public SuperCoder
    {
    public:

        /// @copydoc layer::value_type
        using value_type = typename SuperCoder::value_type;

        /// The storage type used
        using storage_type = sak::const_storage;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            uint32_t max_block_size = the_factory.max_block_size();
            assert(max_block_size > 0);

            // Construct should only be called once so
            // m_data.size() should be zero
            assert(m_data.size() == 0);
            m_data.resize(max_block_size, 0);
        }

        /// @copydoc layer::symbol(uint32_t)
        // uint8_t* symbol(uint32_t index)
        // {
        //     assert(index < SuperCoder::symbols());
        //     return &m_data[index * SuperCoder::symbol_size()];
        // }

        // /// @copydoc layer::symbol_value(uint32_t)
        // auto symbol_value(uint32_t index) ->
        //     decltype(sak::cast_storage<value_type>(()))
        // {
        //     return sak::cast_storage<value_type>(symbol_storage(index));
        // }

        // /// @copydoc layer::symbol_value(uint32_t)
        // auto symbol_value(uint32_t index) ->
        //     decltype(sak::cast_storage<value_type>(storage_type()))
        // {
        //     return sak::cast_storage<value_type>(symbol_storage(index));
        // }

        // /// @copydoc layer::symbol(uint32_t) const
        // const uint8_t* symbol(uint32_t index) const
        // {
        //     assert(index < SuperCoder::symbols());
        //     return &m_data[index * SuperCoder::symbol_size()];
        // }

        /// @copydoc layer::symbol_storage(uint32_t) const
        sak::const_storage symbol_storage(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());

            auto data = m_data.data() + (index * SuperCoder::symbol_size());
            return sak::const_storage(data, SuperCoder::symbol_size());
        }

        /// @copydoc layer::symbol_storage(uint32_t) const
        sak::mutable_storage symbol_storage(uint32_t index)
        {
            assert(index < SuperCoder::symbols());

            auto data = m_data.data() + (index * SuperCoder::symbol_size());
            return sak::mutable_storage(data, SuperCoder::symbol_size());
        }

        /// @copydoc layer::symbol_value(uint32_t) const
        // const value_type* symbol_value(uint32_t index) const
        // {
        //     return reinterpret_cast<const value_type*>(symbol(index));
        // }

        /// @copydoc layer::set_symbols(const sak::const_storage&)
        void set_symbols(const storage_type& symbol_storage)
        {
            assert(symbol_storage.m_size > 0);
            assert(symbol_storage.m_data != 0);
            assert(symbol_storage.m_size <=
                   SuperCoder::symbols() * SuperCoder::symbol_size());

            // Use the copy function
            sak::copy_storage(sak::storage(m_data), symbol_storage);
        }

        /// @copydoc layer::set_symbol(uint32_t, const sak::const_storage&)
        void set_symbol(uint32_t index, const storage_type& symbol)
        {
            assert(symbol.m_data != 0);

            // If the user sets a specific symbol on deep storage we allow that
            // it does not have the full size. In this cases it is the users
            // who must ensure correct decoding
            assert(symbol.m_size <= SuperCoder::symbol_size());
            assert(symbol.m_size > 0);

            assert(index < SuperCoder::symbols());

            sak::mutable_storage dest_data = sak::storage(m_data);

            uint32_t offset = index * SuperCoder::symbol_size();
            dest_data += offset;

            assert(dest_data.m_size >= SuperCoder::symbol_size());

            // Copy the data
            sak::copy_storage(dest_data, symbol);
        }

        /// @copydoc layer::copy_symbols(const sak::mutable_storage&)
        void copy_symbols(const sak::mutable_storage &dest_storage) const
        {
            assert(dest_storage.m_size > 0);
            assert(dest_storage.m_data != 0);

            uint32_t data_to_copy =
                std::min(dest_storage.m_size, SuperCoder::block_size());

            /// Wrap our buffer in a storage object
            sak::const_storage src_storage =
                sak::storage(&m_data[0], data_to_copy);

            /// Use the copy_storage() function to copy the data
            sak::copy_storage(dest_storage, src_storage);
        }

        /// @todo Change to copy_from_symbol
        /// @copydoc layer::copy_symbol(uint32_t,
        ///                             const sak::mutable_storage&)
        void copy_symbol(uint32_t index,
                         const sak::mutable_storage &dest) const
        {
            assert(dest.m_size > 0);
            assert(dest.m_data != 0);

            uint32_t data_to_copy =
                std::min(dest.m_size, SuperCoder::symbol_size());

            sak::const_storage src = symbol_storage(index);
            src.m_size = data_to_copy;

            sak::copy_storage(dest, src);
        }

        /// @copydoc layer::copy_into_symbols(const sak::const_storage&)
        void copy_into_symbols(const sak::const_storage &src)
        {
            // For deep symbol storage this is the same as using
            // layer::set_symbols(const sak::mutable_storage&)
            set_symbols(src);
        }

        /// @copydoc layer::copy_into_symbol(uint32_t,
        ///              const sak::const_storage&)
        void copy_into_symbol(uint32_t index,
                              const sak::const_storage &src)
        {
            // For deep symbol storage this is the same as using
            // layer::set_symbol(uint32_t, const
            // sak::mutable_storage&).
            //
            // This is however not the case for shallow storage where
            // set_symbol will change the underlying pointer of the
            // symbol. Using copy_into_symbol we don't want that - we
            // want to replace the content of where ever the pointer is
            // pointing now.
            set_symbol(index, src);
        }

        /// @return A pointer to the memory stored by the deep storage
        ///         layer
        uint8_t* block_data()
        {
            return m_data.data();
        }

        /// @return A pointer to the memory stored by the deep storage
        ///         layer
        const uint8_t* block_data() const
        {
            return m_data.data();
        }

    private:

        /// Storage for the symbol data
        std::vector<uint8_t> m_data;
    };
}
