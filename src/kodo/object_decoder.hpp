// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <sak/storage.hpp>

#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>

#include "rfc5052_partitioning_scheme.hpp"
#include "final_layer.hpp"
#include "basic_factory.hpp"

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
                m_storage = storage;
            }

            const storage_type& storage() const
            {
                return m_storage;
            }

            uint32_t storage_size() const
            {
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

            m_storage = storage;
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

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_partitioning = partitioning_type(
                the_factory.max_symbols(),
                the_factory.max_symbol_size(),
                the_factory.storage_size());
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
            uint32_t symbols = SuperCoder::symbols(index);
            uint32_t symbol_size = SuperCoder::symbol_size(index);

            m_stack_factory->set_symbols(symbols);
            m_stack_factory->set_symbol_size(symbol_size);

            auto stack = m_stack_factory->build(index);

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

        stack_pointer_type build(uint32_t index)
        {
            auto stack = SuperCoder::build(index);
            assert(stack);

            uint32_t offset = SuperCoder::byte_offset(index);
            uint32_t block_size = SuperCoder::block_size(index);

            auto data = SuperCoder::storage() + offset;
            assert(data.m_size >= block_size);

            // Adjust the size of the decoding buffer to fit this
            // decoder
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







    /// @brief The object decoder supports decoding objects split over
    ///        multiple decoders.
    ///
    /// When an object is too large for a single encoder/decoder pair,
    /// the object decoder uses the specified partitioning scheme to
    /// determine how decoders should be constructed to cover the entire
    /// object.
    template
    <
        class DecoderType,
        class BlockPartitioning
    >
    class object_decoder : boost::noncopyable
    {
    public:

        /// The type of factory used to build decoders
        typedef typename DecoderType::factory factory;

        /// Pointer to a decoder
        typedef typename factory::pointer pointer;

        /// The block partitioning scheme used
        typedef BlockPartitioning block_partitioning;

    public:

        /// Constructs a new object decoder
        /// @param factory The decoder factory to use
        /// @param object_size The size in bytes of the object to be decoded
        object_decoder(factory &decoder_factory, uint32_t object_size)
            : m_factory(decoder_factory),
              m_object_size(object_size)
        {
            assert(m_object_size > 0);

            m_partitioning = block_partitioning(
                m_factory.max_symbols(),
                m_factory.max_symbol_size(),
                m_object_size);

        }

        /// @return The number of decoders which may be created for
        ///         this object
        uint32_t decoders() const
        {
            return m_partitioning.blocks();
        }

        /// Builds a specific decoder
        /// @param decoder_id Specifies the decoder to build
        /// @return The initialized decoder
        pointer build(uint32_t decoder_id)
        {
            assert(decoder_id < m_partitioning.blocks());

            uint32_t symbols =
                m_partitioning.symbols(decoder_id);

            uint32_t symbol_size =
                m_partitioning.symbol_size(decoder_id);

            m_factory.set_symbols(symbols);
            m_factory.set_symbol_size(symbol_size);

            pointer decoder = m_factory.build();

            // Set bytes used
            uint32_t bytes_used =
                m_partitioning.bytes_used(decoder_id);

            decoder->set_bytes_used(bytes_used);

            return decoder;
        }

        /// @return The total size of the object to decode in bytes
        uint32_t object_size() const
        {
            return m_object_size;
        }

    protected:

        /// The decoder factory
        factory &m_factory;

        /// The block partitioning scheme used
        block_partitioning m_partitioning;

        /// Store the total object size in bytes
        uint32_t m_object_size;
    };

}
