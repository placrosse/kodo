// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef KODO_SHALLOW_SYMBOL_STORAGE_HPP
#define KODO_SHALLOW_SYMBOL_STORAGE_HPP

#include <stdint.h>
#include <vector>

#include <boost/shared_array.hpp>

#include <fifi/fifi_utils.hpp>

#include "storage.hpp"

namespace kodo
{
    /// Storage traits class for the const storage
    class shallow_const_trait
    {
    public:

        /// The value type
        typedef uint8_t value_type;
        
        /// value type pointer
        typedef const value_type* value_ptr;

        /// storage type
        typedef const_storage storage_type;

    };

    /// Storage traits class for the mutable storage
    class shallow_mutable_trait
    {
    public:

        /// The value type
        typedef uint8_t value_type;

        /// value type pointer
        typedef value_type* value_ptr;

        /// storage type
        typedef mutable_storage storage_type;

    };

    /// The shallow storage implementation. In this context shallow
    /// means that the symbol storage only contains pointers to some
    /// external data structure. This is useful in cases where data to
    /// be encoded already has been read into memory or if a user requires
    /// incoming data to be directly decoded into a specific buffer.
    template<class StorageTraits, class SuperCoder>
    class shallow_symbol_storage : public SuperCoder
    {
    public:

        /// The storage traits
        typedef StorageTraits storage_trait;
        
        /// The value type used
        typedef typename storage_trait::value_type value_type;
        
        /// The pointer used
        typedef typename storage_trait::value_ptr value_ptr;

        /// The storage type used
        typedef typename storage_trait::storage_type storage_type;

        /// A sequence of storage types
        typedef typename storage_sequence<storage_type>::type
            storage_sequence_type;

    public:

        /// @copydoc final_coder_factory::construct()
        void construct(uint32_t max_symbols, uint32_t max_symbol_size)
            {
                SuperCoder::construct(max_symbols, max_symbol_size);

                m_mapping.resize(max_symbols, 0);
            }

        /// @copydoc final_coder_factory::initialize()
        void initialize(uint32_t symbols, uint32_t symbol_size)
            {
                SuperCoder::initialize(symbols, symbol_size);

                std::fill(m_mapping.begin(), m_mapping.end(), (value_ptr) 0);
            }

        /// @param index the index number of the raw symbol
        /// @return uint8_t pointer to the symbol
        const uint8_t* raw_symbol(uint32_t index) const
            {
                assert(index < SuperCoder::symbols());
                return symbol(index);
            }

        /// @param index the index number of the symbol
        /// @return value_type pointer to the symbol
        value_ptr symbol(uint32_t index) const
            {
                assert(index < SuperCoder::symbols());

                return m_mapping[index];
            }

        /// Sets the storage
        /// @param symbol_storage a const storage container
        void set_symbols(const storage_type &symbol_storage)
            {
                storage_sequence_type symbol_sequence =
                    split_storage(symbol_storage, SuperCoder::symbol_size());

                uint32_t sequence_size = symbol_sequence.size();
                assert(sequence_size == SuperCoder::symbols());

                for(uint32_t i = 0; i < sequence_size; ++i)
                {
                    set_symbol(i, symbol_sequence[i]);
                }
            }

        /// Sets a symbol - by copying it into the right location in the buffer
        /// @param index the index of the symbol into the coding block
        /// @param symbol the actual data of that symbol
        void set_symbol(uint32_t index, const storage_type &symbol)
            {
                assert(symbol.m_data != 0);
//                std::cout << symbol.m_size << " " << SuperCoder::symbol_size() << std::endl;
                assert(symbol.m_size == SuperCoder::symbol_size());
                assert(index < SuperCoder::symbols());

                m_mapping[index] = cast_storage<value_type>(symbol);
            }

        /// Overload of the copy_storage() function for this symbol storage.
        /// @param dest_storage destination buffer
        void copy_symbols(mutable_storage dest_storage)
            {
                assert(dest_storage.m_size > 0);
                assert(dest_storage.m_data != 0);

                uint32_t data_to_copy = std::min(dest_storage.m_size,
                                                 SuperCoder::block_size());

                uint32_t symbol_index = 0;

                while(data_to_copy > 0)
                {
                    uint32_t copy_size = std::min(data_to_copy, SuperCoder::symbol_size());
                    data_to_copy -= copy_size;

                    const_storage src_storage =
                        storage(symbol(symbol_index), copy_size);

                    copy_storage(dest_storage, src_storage);

                    dest_storage.m_size -= copy_size;
                    dest_storage.m_data += copy_size;

                    ++symbol_index;

                }

            }

    protected:

        /// Symbol mapping
        std::vector<value_ptr> m_mapping;

    };

    /// Defines a coding layer for 'const' symbol storage. Only useful
    /// for encoders since these to modify the buffers / data they
    /// operate on.
    template<class SuperCoder>
    class const_shallow_symbol_storage
        : public shallow_symbol_storage<shallow_const_trait, SuperCoder>
    {};

    /// Defines a coding layer for 'mutable' symbol storage. Allows the
    /// buffer data to be modified i.e. useful in decoders which need to
    /// access and modify the incoming symbols
    template<class SuperCoder>
    class mutable_shallow_symbol_storage
        : public shallow_symbol_storage<shallow_mutable_trait, SuperCoder>
    {};

}

#endif

