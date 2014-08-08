// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <vector>

#include "const_shallow_symbol_storage.hpp"

namespace kodo
{
    /// @ingroup storage_layers
    ///
    /// @brief Same as a shallow storage but supports storage object
    ///        with a total size less than the coding block size.
    ///
    /// Essentially the same as the shallow storage however the
    /// shallow storage only allows buffers which are exactly the
    /// size of symbols * symbol_size. The partial buffer allows
    /// blocks of data smaller. I.e. of a block is 10000 bytes and
    /// the data you want to encode is in a 9995 byte buffer you can
    /// use it with the partial symbol storage without having to move
    /// it to a 10000 byte temp. buffer.
    template<class SuperCoder>
    class partial_shallow_symbol_storage : public SuperCoder
    {
    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);

            assert(the_factory.max_symbol_size() > 0);
            m_partial_symbol.reserve(the_factory.max_symbol_size());
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_partial_symbol.resize(the_factory.symbol_size());
            m_partial_symbol_size = 0;
            m_has_partial_symbol = false;
        }

        /// Initializes the symbol storage layer so that the pointers to the
        /// symbol data are valid. Calling this function will work even
        /// without providing data enough to initialize all symbol pointers.
        /// @copydoc layer::set_symbols(const sak::const_storage &)
        template<class StorageType>
        void set_symbols(const StorageType &symbol_storage)
        {
            uint32_t symbol_size = SuperCoder::symbol_size();

            const auto& symbol_sequence =
                sak::split_storage(symbol_storage, symbol_size);

            // Check that we have as many symbols in the sequence as
            // we should
            assert(symbol_sequence.size() == SuperCoder::symbols());

            // Check that cast below is safe
            assert(symbol_sequence.size() <=
                   std::numeric_limits<uint32_t>::max());

            uint32_t sequence_size = (uint32_t)symbol_sequence.size();
            uint32_t last_index = sequence_size - 1;

            for(uint32_t i = 0; i < last_index; ++i)
            {
                SuperCoder::set_symbol(i, symbol_sequence[i]);
            }

            const auto& last_symbol = symbol_sequence[last_index];

            if(last_symbol.m_size < symbol_size)
            {
                const auto& partial_symbol = sak::storage(m_partial_symbol);

                sak::copy_storage(partial_symbol, last_symbol);
                SuperCoder::set_symbol(last_index, partial_symbol);

                // Update our state since we now have a partial symbol
                m_has_partial_symbol = true;
                m_partial_symbol_size = last_symbol.m_size;
            }
            else
            {
                SuperCoder::set_symbol(last_index, last_symbol);
            }
        }

        /// @returns true if the partial symbol buffer is in use
        bool has_partial_symbol() const
        {
            return m_has_partial_symbol;
        }

        /// @returns the size of the valid bytes in the partial symbol buffer
        uint32_t partial_symbol_size() const
        {
            assert(m_has_partial_symbol);
            return m_partial_symbol_size;
        }

        /// @return a pointer to the data of the partial symbol
        const uint8_t* partial_symbol() const
        {
            assert(m_has_partial_symbol);
            return m_partial_symbol.data();
        }

    protected:

        /// The partial symbol
        std::vector<uint8_t> m_partial_symbol;

        /// Keeps track of whether the "partial symbol" buffer is in use
        bool m_has_partial_symbol;

        /// Keeps track of how much of the "partial symbol" buffer is used
        uint32_t m_partial_symbol_size;
    };
}
