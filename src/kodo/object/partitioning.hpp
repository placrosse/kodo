// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
namespace object
{
    /// @todo docs
    template<class PartitioningScheme, class SuperCoder>
    class partitioning : public SuperCoder
    {
    public:

        /// The partitioning scheme used
        using partitioning_scheme_type = PartitioningScheme;

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

                partitioning_scheme_type p(m_symbols, m_symbol_size, object_size);
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

            m_partitioning_scheme = partitioning_scheme_type(
                the_factory.symbols(),
                the_factory.symbol_size(),
                the_factory.object_size());
        }

        const partitioning_scheme_type& partitioning_scheme() const
        {
            return m_partitioning_scheme;
        }

        uint32_t blocks() const
        {
            return m_partitioning_scheme.blocks();
        }

        uint32_t symbols(uint32_t index)
        {
            return m_partitioning_scheme.symbols(index);
        }

        uint32_t symbol_size(uint32_t index)
        {
            return m_partitioning_scheme.symbol_size(index);
        }

        uint32_t bytes_used(uint32_t index)
        {
            return m_partitioning_scheme.bytes_used(index);
        }

        uint32_t byte_offset(uint32_t index)
        {
            return m_partitioning_scheme.byte_offset(index);
        }

        uint32_t block_size(uint32_t index)
        {
            return m_partitioning_scheme.block_size(index);
        }

    protected:

        partitioning_scheme_type m_partitioning_scheme;

    };
}
}
