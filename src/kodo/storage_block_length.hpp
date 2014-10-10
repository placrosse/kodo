// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <fifi/fifi_utils.hpp>

namespace kodo
{
    /// @ingroup symbol_storage_layers
    ///
    /// @brief Defines several functions to retrieve info about the
    ///        symbol storage layers.
    template<class SuperCoder>
    class storage_block_length : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

    public:

        /// @ingroup factory_base_layers
        /// @brief Provides access to the maximum symbol and symbol size
        ///        information.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// Constructor
            /// @param max_symbols the maximum symbols this coder can expect
            /// @param max_symbol_size the maximum size of a symbol in bytes
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory_base::max_symbol_length() const
            uint32_t max_symbol_length() const
            {
                return fifi::size_to_length<field_type>(
                    SuperCoder::factory_base::max_symbol_size());
            }

            /// @copydoc layer::factory_base::max_block_length() const
            uint32_t max_block_length() const
            {
                return fifi::size_to_length<field_type>(
                    SuperCoder::factory_base::max_block_size());
            }

            /// @copydoc layer::factory_base::symbol_length() const;
            uint32_t symbol_length() const
            {
                return fifi::size_to_length<field_type>(
                    SuperCoder::factory_base::symbol_size());
            }
        };

    public:

        /// @copydoc layer::symbol_length() const
        uint32_t symbol_length() const
        {
            return fifi::size_to_length<field_type>(SuperCoder::symbol_size());
        }

        /// @copydoc layer::block_length() const
        uint32_t block_length() const
        {
            return fifi::size_to_length<field_type>(SuperCoder::block_size());
        }
    };
}
