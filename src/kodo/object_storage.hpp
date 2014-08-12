// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <sak/storage.hpp>

#include "rfc5052_partitioning_scheme.hpp"
#include "final_layer.hpp"
#include "basic_factory.hpp"
#include "storage_block_size.hpp"

namespace kodo
{
    /// @todo + docs
    ///
    /// @brief Provides storage and access to the memory used by the object.
    ///
    /// When encoding or decoding an object we will be working with
    /// more memory than what can contained within a single encoder or
    /// decoder. The object stoarge layer contains a StorageType
    /// object wich provides information about the memory we are
    /// either encoding or decoder. The StorageType will typically be
    /// either a sak::mutable_st
    template<class StorageType, class SuperCoder>
    class object_storage : public SuperCoder
    {
    public:

        /// The storage type
        using storage_type = StorageType;

        /// The type of the pointer to the stack we are building
        using stack_pointer_type = typename SuperCoder::stack_pointer_type;

    public:

        /// @ingroup factory_base_layers
        ///
        /// @brief
        class factory_base : public SuperCoder::factory_base
        {
        public:

            factory_base(uint32_t symbols, uint32_t symbol_size)
                : SuperCoder::factory_base(symbols, symbol_size)
            { }

            void set_storage(const storage_type& storage)
            {
                assert(storage.m_data != 0);
                assert(storage.m_size > 0);

                m_storage = storage;
            }

            /// @return the stored storage object
            const storage_type& storage() const
            {
                assert(m_storage.m_data != 0);
                assert(m_storage.m_size > 0);

                return m_storage;
            }

            /// @return the size of the object storage
            uint32_t object_size() const
            {
                assert(m_storage.m_size > 0);

                return m_storage.m_size;
            }

        protected:

            /// The storage of the object to encode
            storage_type m_storage;
        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_storage = the_factory.storage();
        }

        /// @param index Index of the block to build an encoder or
        ///        decoder stack for
        ///
        /// @return the newly built encoder or decoder initialized
        ///         with storage through the the set_symbols function
        stack_pointer_type build(uint32_t index)
        {
            auto stack = SuperCoder::build(index);
            assert(stack);

            uint32_t offset = SuperCoder::byte_offset(index);
            uint32_t block_size = SuperCoder::block_size(index);

            storage_type data = m_storage;
            assert(data.m_data != 0);
            assert(data.m_size > offset);

            // Adjust the size of the buffer to fit this stack
            data.m_data += offset;
            data.m_size -= offset;

            // Make sure we do not exceed the block size
            data.m_size = std::min(block_size, data.m_size);

            stack->set_symbols(data);

            return stack;
        }

        /// @return the storage used for this object
        const storage_type& storage() const
        {
            return m_storage;
        }

    protected:

        /// Stores the storage for the object being encoded or decoded
        storage_type m_storage;
    };
}
