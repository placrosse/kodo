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
    template<class StorageType, class SuperCoder>
    class object_storage : public SuperCoder
    {
    public:

        using storage_type = StorageType;

    public:

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

            const storage_type& storage() const
            {
                assert(m_storage.m_data != 0);
                assert(m_storage.m_size > 0);

                return m_storage;
            }

            uint32_t object_size() const
            {
                assert(m_storage.m_size > 0);

                return m_storage.m_size;
            }

        protected:

            storage_type m_storage;
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_storage = the_factory.storage();
        }

        const storage_type& storage() const
        {
            return m_storage;
        }

    protected:

        storage_type m_storage;
    };

    template<class SuperCoder>
    using mutable_object_storage =
        object_storage<sak::mutable_storage, SuperCoder>;

    template<class SuperCoder>
    using const_object_storage =
        object_storage<sak::const_storage, SuperCoder>;


    template<class Partitioning, class SuperCoder>
    class object_partitioning : public SuperCoder
    {
    public:

        /// The partitioning scheme used
        using partitioning_type = Partitioning;

    public:

        class factory_base : public SuperCoder::factory_base
        {
        public:

            factory_base(uint32_t symbols, uint32_t symbol_size)
                : SuperCoder::factory_base(symbols, symbol_size),
                  m_symbols(symbols),
                  m_symbol_size(symbol_size)
            {
                assert(m_symbols > 0);
                assert(m_symbol_size > 0);
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size;
            }

            /// This is an unfortunate consequence of not having
            /// support for variable symbol sizes. Since all
            /// "generations" are a multiple of the symbol size it
            /// means that if the object we are try to decode is not a
            /// multiple of the symbol size we have to allocate more
            /// space than the actual object so that the decoder will
            /// not touch invalid memory.
            ///
            /// Even with a single generation this is a problem
            /// imagine have a symbol size of 1400 and 10 symbols but
            /// the object is only 13900 bytes, then we still have to
            /// make a buffer of 14000 bytes to make the decoding
            /// algorithms work.
            /// @todo To be fixed :)
            uint32_t total_block_size(uint32_t object_size) const
            {
                assert(object_size > 0);

                partitioning_type p(m_symbols, m_symbol_size, object_size);
                return p.total_block_size();
            }

        protected:

            uint32_t m_symbols;
            uint32_t m_symbol_size;
        };


    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_partitioning = partitioning_type(
                the_factory.symbols(),
                the_factory.symbol_size(),
                the_factory.object_size());
        }

        const partitioning_type& partitioning() const
        {
            return m_partitioning;
        }

        uint32_t blocks() const
        {
            return m_partitioning.blocks();
        }

        uint32_t symbols(uint32_t index)
        {
            return m_partitioning.symbols(index);
        }

        uint32_t symbol_size(uint32_t index)
        {
            return m_partitioning.symbol_size(index);
        }

        uint32_t bytes_used(uint32_t index)
        {
            return m_partitioning.bytes_used(index);
        }

        uint32_t byte_offset(uint32_t index)
        {
            return m_partitioning.byte_offset(index);
        }

        uint32_t block_size(uint32_t index)
        {
            return m_partitioning.block_size(index);
        }

    protected:

        partitioning_type m_partitioning;

    };

    template<class SuperCoder>
    using rfc5052_object_partitioning =
        object_partitioning<rfc5052_partitioning_scheme, SuperCoder>;

    template<class Stack, class SuperCoder>
    class object_stack : public SuperCoder
    {
    public:
        using stack_type = Stack;
    };

    template<template <class> class StackWrapper, class SuperCoder>
    class extend_object_stack : public SuperCoder
    {
    public:
        using stack_type = StackWrapper<typename SuperCoder::stack_type>;
    };

    template<class SuperCoder>
    class factory_object_stack : public SuperCoder
    {
    public:

        using stack_type = typename SuperCoder::stack_type;
        using stack_factory_type = typename stack_type::factory;
        using stack_pointer_type = typename stack_factory_type::pointer;
        using stack_factory_pointer_type = std::shared_ptr<stack_factory_type>;

    public:

        class factory_base : public SuperCoder::factory_base
        {
        public:

            factory_base(uint32_t symbols, uint32_t symbol_size)
                : SuperCoder::factory_base(symbols, symbol_size),
                  m_stack_factory(std::make_shared<stack_factory_type>(
                                      symbols, symbol_size))
            { }

            stack_factory_pointer_type stack_factory()
            {
                return m_stack_factory;
            }

            stack_factory_pointer_type m_stack_factory;
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_stack_factory = the_factory.stack_factory();
        }

        stack_pointer_type build(uint32_t index)
        {
            // Get the symbols and symbol size from the partitioning
            // scheme for this specific index
            uint32_t symbols = SuperCoder::symbols(index);
            uint32_t symbol_size = SuperCoder::symbol_size(index);

            m_stack_factory->set_symbols(symbols);
            m_stack_factory->set_symbol_size(symbol_size);

            auto stack = m_stack_factory->build();

            assert(stack);
            assert(stack->symbols() == symbols);
            assert(stack->symbol_size() == symbol_size);

            stack->set_bytes_used(SuperCoder::bytes_used(index));
            return stack;
        }

    protected:

        stack_factory_pointer_type m_stack_factory;
    };

    template<class SuperCoder>
    class set_symbols_object_storage : public SuperCoder
    {
    public:

        using stack_pointer_type = typename SuperCoder::stack_pointer_type;

    public:

        stack_pointer_type build(uint32_t index)
        {
            auto stack = SuperCoder::build(index);
            assert(stack);

            uint32_t offset = SuperCoder::byte_offset(index);
            uint32_t block_size = SuperCoder::block_size(index);

            auto data = SuperCoder::storage();
            assert(data.m_data != 0);
            assert(data.m_size >= block_size);

            // Adjust the size of the buffer to fit this stack
            data += offset;

            /// @todo This assert triggers because the encoder
            /// supports not "full" generations but the decoder does
            /// not. So for decoding we allocate sometimes more memory
            /// than we need, but for the encoder we do not. However,
            /// then this assert fails for the encoder eventhough the
            /// code works correctly
            assert(data.m_size >= block_size);

            data.m_size = block_size;
            stack->set_symbols(data);

            return stack;
        }
    };

    template<class Stack>
    class new_storage_decoder : public
        set_symbols_object_storage<
        factory_object_stack<
        object_stack<Stack,
        rfc5052_object_partitioning<
        mutable_object_storage<
        final_layer> > > > >
    {
    public:
        using factory = basic_factory<new_storage_decoder>;
    };

    template<class Stack>
    class new_storage_encoder : public
        set_symbols_object_storage<
        factory_object_stack<
        object_stack<Stack,
        rfc5052_object_partitioning<
        const_object_storage<
        final_layer> > > > >
    {
    public:
        using factory = basic_factory<new_storage_encoder>;
    };


}
