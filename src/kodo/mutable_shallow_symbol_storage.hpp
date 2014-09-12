// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "shallow_symbol_storage.hpp"

namespace kodo
{
    /// @ingroup symbol_storage_layers
    ///
    /// @brief Defines a coding layer for 'mutable' symbol
    ///        storage. Allows the buffer data to be modified
    ///        i.e. useful in decoders which need to access and modify
    ///        the incoming symbols
    template<class SuperCoder>
    class mutable_shallow_symbol_storage : public
        shallow_symbol_storage<sak::mutable_storage, SuperCoder>
    {
    public:

        /// The actual SuperCoder type
        typedef shallow_symbol_storage<sak::mutable_storage, SuperCoder> Super;

        /// @copydoc layer::value_type
        typedef typename Super::value_type value_type;

    protected:

        /// Access to the symbol pointers
        using Super::m_data;

    public:

        /// @copydoc layer::symbol(uint32_t)
        uint8_t* symbol(uint32_t index)
        {
            assert(index < Super::symbols());

            // Did you forget to set the symbol
            assert(m_data[index]);

            return m_data[index];
        }

        /// @copydoc layer::symbol_value(uint32_t)
        value_type* symbol_value(uint32_t index)
        {
            return reinterpret_cast<value_type*>(symbol(index));
        }

        /// @copydoc layer::copy_into_symbols(const sak::mutable_storage&)
        void copy_into_symbols(const sak::const_storage &src)
        {
            auto symbol_sequence = sak::split_storage(
                src, Super::symbol_size());

            uint32_t sequence_size = symbol_sequence.size();
            assert(sequence_size == Super::symbols());

            for(uint32_t i = 0; i < sequence_size; ++i)
            {
                copy_into_symbol(i, symbol_sequence[i]);
            }
        }

        /// @copydoc layer::copy_into_symbol(uint32_t,
        ///              const sak::mutable_storage&)
        void copy_into_symbol(uint32_t index,
                              const sak::const_storage &src)
        {
            assert(src.m_data != 0);
            assert(src.m_size == Super::symbol_size());
            assert(index < Super::symbols());
            assert(Super::is_symbol_available(index));

            sak::mutable_storage dest =
                sak::storage(symbol(index), Super::symbol_size());

            sak::copy_storage(dest, src);
        }
    };
}
