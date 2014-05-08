// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../nested_stack.hpp"

namespace kodo
{

    template<class SuperCoder>
    class fulcrum_nested_stack_info : public SuperCoder
    {
    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            uint32_t nested_max_symbols() const
            {
                return SuperCoder::factory::max_symbols() +
                    SuperCoder::factory::max_expansion();
            }

            uint32_t nested_max_symbol_size() const
            {
                return SuperCoder::factory::max_symbol_size();
            }

        };

    public:

        uint32_t nested_symbols() const
        {
            return SuperCoder::symbols() + SuperCoder::expansion();
        }

        uint32_t nested_symbol_size() const
        {
            return SuperCoder::symbol_size();
        }
    };

        /// @todo docs + test
    template
    <
        class NestedStack,
        class SuperCoder
    >
    using fulcrum_nested_stack =
        nested_stack<NestedStack,
        fulcrum_nested_stack_info<SuperCoder> >;

    // /// @todo review + docs + test
    // ///
    // /// The anycode_stack layer ensures that the inner code (which is
    // /// is stored as a nested stack) is initialized with the correct
    // /// symbols and symbol size. In the Fulcrum code the inner code
    // /// will encode / decoder over symbols + the outer code expansion.
    // ///
    // /// This layer should be added to the outer code stack (which is what
    // /// the user sees).
    // template
    // <
    //     class NestedStack,
    //     class SuperCoder
    // >
    // class fulcrum_nested_stack : public SuperCoder
    // {
    // public:

    //     /// The nested stack type
    //     typedef NestedStack nested_stack_type;

    //     /// The nested code factory
    //     typedef typename nested_stack_type::factory nested_factory_type;

    //     /// The nested code pointer
    //     typedef typename nested_stack_type::pointer nested_pointer;

    //     /// Proxy stack
    //     typedef NestedStack proxy_stack_type;

    // public:

    //     /// @ingroup factory_layers
    //     /// The factory layer associated with this coder.
    //     class factory : public SuperCoder::factory
    //     {
    //     public:

    //         /// Ensure that the inner code is create over the expansion
    //         /// selected.
    //         /// @copydoc layer::factory::factory(uint32_t,uint32_t)
    //         factory(uint32_t max_symbols, uint32_t max_symbol_size) :
    //             SuperCoder::factory(max_symbols, max_symbol_size),
    //             m_nested_factory(
    //                 max_symbols + SuperCoder::factory::max_expansion(),
    //                 max_symbol_size)
    //         { }

    //         /// @return A reference to the nested factory
    //         nested_factory_type& nested()
    //         {
    //             return m_nested_factory;
    //         }

    //         /// @return A reference to the nested factory
    //         const nested_factory_type& nested() const
    //         {
    //             return m_nested_factory;
    //         }

    //     private:

    //         /// Factory for the nested code
    //         nested_factory_type m_nested_factory;

    //     };

    //     /// @copydoc layer::initialize(Factory&)
    //     template<class Factory>
    //     void initialize(Factory& the_factory)
    //     {
    //         SuperCoder::initialize(the_factory);

    //         auto& nested_factory = the_factory.nested();

    //         nested_factory.set_symbols(
    //             the_factory.symbols() + the_factory.expansion());

    //         nested_factory.set_symbol_size(the_factory.symbol_size());

    //         if(!m_nested_stack)
    //         {
    //             m_nested_stack = nested_factory.build();
    //         }
    //         else
    //         {
    //             m_nested_stack->initialize(nested_factory);
    //         }

    //     }

    //     /// @return A reference to the nested encoder
    //     nested_pointer& nested()
    //     {
    //         assert(m_nested_stack);
    //         return m_nested_stack;
    //     }

    //     /// @return A reference to the nested encoder
    //     const nested_pointer& nested() const
    //     {
    //         assert(m_nested_stack);
    //         return m_nested_stack;
    //     }

    // protected:

    //     /// Pointer to the nested decoder
    //     nested_pointer m_nested_stack;

    // };

}
