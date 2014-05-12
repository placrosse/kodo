// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @todo docs + test
    /// @ingroup utility
    ///
    /// @brief When using nested stacks this layer controls the coding
    ///        parameters used.
    ///
    /// When embedding a nested stack we need to provide the same
    /// basic information we need for any codec stack.
    ///
    /// For the stack factory:
    /// - The maximum number of symbols
    /// - The maximum size of a symbol
    ///
    /// For the stack:
    /// - The number of symbols
    /// - The size of each symbol
    ///
    /// This layer will utilize the same configuration as used in the
    /// main stack. But by replacing this layer we may customize those
    /// configurations to build other nested codec stacks.
    ///
    /// The nested_stack layer will query this information through the
    /// nested_max_symbols(), nested_max_symbol_size() etc. See
    /// nested_stack.hpp for more information.
    template<class SuperCoder>
    class basic_nested_stack_info : public SuperCoder
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

            /// @return The maximum number of symbols that the nested
            ///         stack should support
            uint32_t nested_max_symbols() const
            {
                return SuperCoder::factory::max_symbols();
            }

            /// @return The symbol size in bytes that the nested stack
            ///         should support
            uint32_t nested_max_symbol_size() const
            {
                return SuperCoder::factory::max_symbol_size();
            }

        };

    public:

        /// @return The number of symbols that the nested stack should
        ///         be configured with
        uint32_t nested_symbols() const
        {
            return SuperCoder::symbols();
        }

        /// @return The symbol size in bytes that the nested stack should
        ///         be configured with
        uint32_t nested_symbol_size() const
        {
            return SuperCoder::symbol_size();
        }
    };
}
