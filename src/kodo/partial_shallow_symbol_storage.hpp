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
    /// @brief "Add-on" to a shallow storage layer which supports
    ///        storage object with a total size less than the coding
    ///        block size.
    ///
    /// The shallow storage layers only allow buffers which are
    /// exactly the symbols * symbol_size in size. Adding this layers
    /// we can allow blocks of data smaller. For example if a block is
    /// 10000 bytes and the data you want to encode is in a 9995 byte
    /// buffer you can use it with the partial symbol storage without
    /// having to move it to a 10000 byte temp. buffer.
    ///
    /// To illustrate how it works lets say we have chosen a symbol
    /// size of 1000 bytes and we want to encode/decode 10 such
    /// symbols. This means that our coding block is 10*1000 bytes.
    ///
    /// The encoding/decoding algorithms will expect that the user
    /// provides 10 symbols of exactly 1000 bytes. If not they will
    /// start to access memory out-of-bounds. So if we wanted to
    /// encode/decode into a buffer of only 9995 bytes this layer
    /// would compensate by detecting this and using its own internal
    /// symbol buffer for the last symbol.
    ///
    /// If this is used with a decoder data will be decoded into the
    /// internal buffer to make sure this is copied to the user's
    /// buffer once decoding is complete you can use the
    /// restore_partial_symbol function.
    template<class SuperCoder>
    class partial_shallow_symbol_storage : public SuperCoder
    {
    public:

        /// Get the storage type used
        using storage_type = typename SuperCoder::storage_type;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);

            assert(the_factory.max_symbol_size() > 0);
            m_internal_symbol.reserve(the_factory.max_symbol_size());
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_internal_symbol.resize(the_factory.symbol_size());
            m_has_partial_symbol = false;
        }

        /// @copydoc layer::set_symbols(const storage_type &)
        void set_symbols(const storage_type &symbol_storage)
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
                const auto& internal_symbol = sak::storage(m_internal_symbol);

                /// @todo This copy step is not needed on decoders and
                ///       could potentially be removed if we has a
                ///       reliable way of detecting that we were in a
                ///       decoder stack.
                sak::copy_storage(internal_symbol, last_symbol);
                SuperCoder::set_symbol(last_index, internal_symbol);

                // Update our state since we now have a partial symbol
                m_has_partial_symbol = true;
                m_partial_symbol = last_symbol;
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

        /// This function will copy the content of the internal buffer
        /// to the user provided partial symbol. When using this layer
        /// in decoders this function makes sure that the user will
        /// get all the decoded data. To invoke the restore
        /// functionality automatically you can use the
        /// restore_partial_symbol_decoder layers.
        void restore_partial_symbol() const
        {
            auto internal_symbol = sak::storage(m_internal_symbol);

            // Adjust the size of the internal symbol so that we only
            // copy the amount needed by the partial symbol
            internal_symbol.m_size = m_partial_symbol.m_size;

            sak::copy_storage(m_partial_symbol, internal_symbol);
        }

    protected:

        /// The internal symbol buffer
        std::vector<uint8_t> m_internal_symbol;

        /// Keeps track of whether the "partial symbol" buffer is in use
        bool m_has_partial_symbol;

        /// The partial symbol
        storage_type m_partial_symbol;
    };
}
