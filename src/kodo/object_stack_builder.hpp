// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>
#include <memory>

namespace kodo
{
    /// @todo docs + tests
    ///
    /// @brief Stores a factory for the stack type.
    ///
    /// The purpose of this layer is to embed it into an "object
    /// stack" to allow the object stack to build the
    /// encoders/decoders needed for encoding/decoding the object. The
    /// layer uses the stack_type provided by the base class
    /// SuperCoder. Using the embeeded factory type the
    /// object_stack_factory will create on stack factory which will
    /// be shared between object stacks.
    template<class SuperCoder>
    class object_stack_builder : public SuperCoder
    {
    public:

        /// The stack that we are building
        using stack_type = typename SuperCoder::stack_type;

        /// The factory of the stack
        using stack_factory_type = typename stack_type::factory;

        /// Pointer produced by the stack factory
        using stack_pointer_type = typename stack_factory_type::pointer;

    public:

        /// @ingroup factory_base_layers
        ///
        /// @brief Provides access the factory use to build the stack type.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @todo make sure the factory_base constructor is
            /// documented in the layer
            ///
            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t symbols, uint32_t symbol_size)
                : SuperCoder::factory_base(symbols, symbol_size),
                  m_stack_factory(std::make_shared<stack_factory_type>(
                                      symbols, symbol_size))
            { }

            /// @return The stack factory
            std::shared_ptr<stack_factory_type> stack_factory()
            {
                return m_stack_factory;
            }

            std::shared_ptr<stack_factory_type> m_stack_factory;
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

            assert(symbols > 0);
            assert(symbol_size > 0);

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

        std::shared_ptr<stack_factory_type> m_stack_factory;
    };
}
