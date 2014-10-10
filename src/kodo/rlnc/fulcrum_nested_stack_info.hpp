// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../nested_stack.hpp"

namespace kodo
{
    /// @todo docs + tests
    ///
    /// @brief This class provides the functions needed by the
    ///        nested_stack layer to correctly initialize a fulcum
    ///        nested stack. The nested stack in fulcrum is typically
    ///        used to only expose the outer code to the user and
    ///        "hide" the inner code as a nested stack.
    ///
    /// We store the parameters as local variables here to rely less
    /// on SuperCoder information, this gives more freedom to where in
    /// the codec stack this layer can be placed.
    template<class SuperCoder>
    class fulcrum_nested_stack_info : public SuperCoder
    {
    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory_base(max_symbols, max_symbol_size),
                m_max_nested_symbols(
                    max_symbols + SuperCoder::factory_base::max_expansion()),
                m_max_nested_symbol_size(max_symbol_size)
            {
                assert(m_max_nested_symbols > 0);
                assert(m_max_nested_symbol_size > 0);
            }

            /// @copydoc layer::factory_base::max_nested_symbols() const
            uint32_t nested_max_symbols() const
            {
                return m_max_nested_symbols;
            }

            /// @copydoc layer::factory_base::max_nested_symbol_size() const
            uint32_t nested_max_symbol_size() const
            {
                return m_max_nested_symbol_size;
            }

            /// Stores the maximum number of symbols which the nested
            /// stack should use
            uint32_t m_max_nested_symbols;

            /// Stores the maximum size of a symbol in the nested stack
            uint32_t m_max_nested_symbol_size;
        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_nested_symbols = the_factory.symbols() + the_factory.expansion();
            m_nested_symbol_size = the_factory.symbol_size();

            assert(m_nested_symbols > 0);
            assert(m_nested_symbol_size > 0);
        }

        /// @copydoc nested_stack_info::nested_symbols() const
        uint32_t nested_symbols() const
        {
            return m_nested_symbols;
        }

        /// @copydoc nested_stack_info::nested_symbol_size() const
        uint32_t nested_symbol_size() const
        {
            return m_nested_symbol_size;
        }

    protected:

        /// Stores the number of symbols which the nested stack should use
        uint32_t m_nested_symbols;

        /// Stores the size of a symbol in the nested stack
        uint32_t m_nested_symbol_size;

    };
}
