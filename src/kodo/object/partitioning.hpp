// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
namespace object
{
    /// @ingroup object_layers
    ///
    /// @brief The partitioning layer exposes the partitioning scheme
    ///        into the object stack. The partitioning scheme defines
    ///        how we segment a large object into blocks and symbols.
    template<class BlockPartitioning, class SuperCoder>
    class partitioning : public SuperCoder
    {
    public:

        /// The partitioning scheme used
        using partitioning_scheme_type = BlockPartitioning;

    public:

        /// @ingroup factory_base_layers
        class factory_base : public SuperCoder::factory_base
        {
        public:

            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size),
                  m_max_symbols(max_symbols),
                  m_max_symbol_size(max_symbol_size)
            {
                assert(m_max_symbols > 0);
                assert(m_max_symbol_size > 0);
            }

            /// @copydoc layer::factory_base::max_symbols() const
            uint32_t max_symbols() const
            {
                return m_max_symbols;
            }

            /// @copydoc layer::factory_base::max_symbol_size() const
            uint32_t max_symbol_size() const
            {
                return m_max_symbol_size;
            }

        protected:
            /// The maximum number of symbols
            uint32_t m_max_symbols;

            /// The maximum symbol size
            uint32_t m_max_symbol_size;
        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_partitioning_scheme = partitioning_scheme_type(
                the_factory.max_symbols(),
                the_factory.max_symbol_size(),
                the_factory.object_size());
        }

        /// @return The partitioning scheme we use
        const partitioning_scheme_type& partitioning_scheme() const
        {
            return m_partitioning_scheme;
        }

        /// @copydoc block_partitioning::blocks() const
        uint32_t blocks() const
        {
            return m_partitioning_scheme.blocks();
        }

        /// @copydoc block_partitioning::symbols(uint32_t) const
        uint32_t symbols(uint32_t index) const
        {
            return m_partitioning_scheme.symbols(index);
        }

        /// @copydoc block_partitioning::symbol_size(uint32_t) const
        uint32_t symbol_size(uint32_t index) const
        {
            return m_partitioning_scheme.symbol_size(index);
        }

        /// @copydoc block_partitioning::bytes_used(uint32_t) const
        uint32_t bytes_used(uint32_t index) const
        {
            return m_partitioning_scheme.bytes_used(index);
        }

        /// @copydoc block_partitioning::bytes_offset(uint32_t) const
        uint32_t byte_offset(uint32_t index) const
        {
            return m_partitioning_scheme.byte_offset(index);
        }

        /// @copydoc block_partitioning::block_size(uint32_t) const
        uint32_t block_size(uint32_t index) const
        {
            return m_partitioning_scheme.block_size(index);
        }

    protected:

        /// The partitioning scheme we have chosen
        partitioning_scheme_type m_partitioning_scheme;
    };
}
}
