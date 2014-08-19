// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @todo + docs
    ///
    /// @brief Provides storage and access to the memory used by the object.
    ///
    /// The object storage layer contains a StorageType object which
    /// provides information about the memory we are either encoding
    /// or decoding. The StorageType will typically be either a
    /// sak::mutable_storage or a sak::const_storage object depending
    /// on whether it is used for encoding or decoding.
    template<class SuperCoder>
    class object_storage : public SuperCoder
    {
    public:

        /// The stack used
        using stack_type = typename SuperCoder::stack_type;

        /// The storage type
        using storage_type = typename stack_type::storage_type;

    public:

        /// @ingroup factory_base_layers
        ///
        /// @brief The factory layer allows a user to specify the
        ///        storage that should be used for the
        ///        encoding/decoding.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t symbols, uint32_t symbol_size)
                : SuperCoder::factory_base(symbols, symbol_size)
            { }

            /// @param storage The object storage that should be used
            void set_storage(const storage_type& storage)
            {
                assert(storage.m_data != 0);
                assert(storage.m_size > 0);

                m_storage = storage;
            }

            /// @return The stored storage object
            const storage_type& storage() const
            {
                assert(m_storage.m_data != 0);
                assert(m_storage.m_size > 0);

                return m_storage;
            }

            /// @return The size of the object storage
            uint32_t object_size() const
            {
                assert(m_storage.m_size > 0);
                return m_storage.m_size;
            }

        protected:

            /// The storage of the object to encode
            storage_type m_storage;
        };

    private:

        template<class T>
        using return_build_type = decltype(std::declval<T>().build(0));

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
        auto build(uint32_t index) -> return_build_type<SuperCoder>
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
