// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    template<class SuperCoder>
    class available_deep_symbol_storage : public SuperCoder
    {
    public:

        /// @copydoc layer::symbols_available() const
        uint32_t symbols_available() const
        {
            return SuperCoder::symbols();
        }

        /// @copydoc layer::is_symbols_available() const
        bool is_symbols_available() const
        {
            return true;
        }

        /// @copydoc layer::is_symbol_available(uint32_t) const
        bool is_symbol_available(uint32_t symbol_index) const
        {
            (void) symbol_index;
            return true;
        }
    };



    /// @todo docs + test
    template<class SuperCoder>
    class mutable_symbol_data : public SuperCoder
    {
    public:

        /// @copydoc layer::symbols_available() const
        uint8_t* symbol(uint32_t index)
        {
            return sak::cast_storage<uint8_t>(
                SuperCoder::symbol_storage(index));
        }

    };

    template<class SuperCoder>
    class const_symbol_data : public SuperCoder
    {
    public:

        /// @copydoc layer::symbols_available() const
        const uint8_t* symbol(uint32_t index) const
        {
            return sak::cast_storage<uint8_t>(
                SuperCoder::symbol_storage(index));
        }

    };

    template<class SuperCoder>
    class const_symbol_value : public SuperCoder
    {
    public:

        /// @copydoc layer::value_type
        using value_type = typename SuperCoder::value_type;

    public:

        /// @copydoc layer::symbols_available() const
        const value_type* symbol_value(uint32_t index) const
        {
            return sak::cast_storage<uint8_t>(
                SuperCoder::symbol_storage(index));
        }

    };


    template<class SuperCoder>
    class mutable_symbol_value : public SuperCoder
    {
    public:

        /// @copydoc layer::value_type
        using value_type = typename SuperCoder::value_type;

    public:

        /// @copydoc layer::symbols_available() const
        value_type* symbol_value(uint32_t index)
        {
            return sak::cast_storage<value_type>(
                SuperCoder::symbol_storage(index));
        }

    };

    template<class SuperCoder>
    class copy_from_symbol_storage : public SuperCoder
    {
    public:

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


    };


}
