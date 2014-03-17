// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

namespace kodo
{

    /// The nested stack layer provides a way of "nesting" a stack
    /// within another stack. Essentially this layer will build and
    /// store another stack as a member. Other layers may access the
    /// nested stack through the nested() and nested() functions
    /// available in the factory and the layer.
    ///
    /// This is useful in cases where we want to redirect some calls
    /// to e.g. a different encoder or decoder depending on some
    /// policy.
    template
    <
        class NestedStack,
        class SuperCoder
    >
    class nested_stack : public SuperCoder
    {
    public:

        /// Typedef of the nested stack 
        typedef NestedStack nested_stack_type;

        /// The nested code factory
        typedef typename nested_stack_type::factory nested_factory_type;

        /// The nested code pointer
        typedef typename nested_stack_type::pointer nested_pointer;

    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory(max_symbols, max_symbol_size),
                m_nested_factory(max_symbols, max_symbol_size)
            { }

        protected:

            /// Make the layer a friend of the factory
            friend class nested_stack;

            /// @return A reference to the nested factory
            nested_factory_type& nested()
            {
                assert(m_nested_factory);
                return m_nested_factory;
            }

            /// @return A reference to the nested factory
            const nested_factory_type& nested() const
            {
                assert(m_nested_factory);
                return m_nested_factory;
            }

        private:

            /// Factory for the nested code
            nested_factory_type m_nested_factory;

        };

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            auto& nested_factory = the_factory.nested();
            assert(nested_factory);

            nested_factory->set_symbols(the_factory.symbols());
            nested_factory->set_symbol_size(the_factory.symbol_size());

            if(!m_nested_stack)
            {
                m_nested_stack = nested_factory->build();
            }
            else
            {
                m_nested_stack->initialize(nested_factory);
            }

        }

        /// @return A reference to the nested encoder
        nested_pointer& nested()
        {
            assert(m_nested_stack);
            return m_nested_stack;
        }

        /// @return A reference to the nested encoder
        const nested_pointer& nested() const
        {
            assert(m_nested_stack);
            return m_nested_stack;
        }


    protected:

        /// Pointer to the nested decoder
        nested_pointer m_nested_stack;

    };

}






