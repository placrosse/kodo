// Copyright Steinwurf Apes 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <boost/dynamic_bitset.hpp>

namespace kodo
{

    /// @todo document this
    ///
    /// @brief This class is used to detect whether the encoder is in
    /// the systematic phase i.e. whether the next symbol to encode
    /// should be a uncoded systematic symbol.
    template<class SuperCoder>
    class storage_aware_systematic_phase : public SuperCoder
    {
    public:

        /// @copdydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_systematic_symbols_sent.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_offset = 0;
            m_systematic_count = 0;

            // Resize here will not hurt performance since the
            // layer::construct(Factory&) function will resize to the
            // maximum size.
            m_systematic_symbols_sent.resize(the_factory.symbols(), false);
        }

        /// @copydoc layer::encode_symbol(uint8_t*,uint32_t)
        void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            asssert(symbol_index < m_systematic_symbols_sent.size());

            SuperCoder::encode_symbol(symbol_data, symbol_index);

            // If we have an one bit the symbol has been sent as systematic
            // so:
            // 1) If the symbol has not been sent before is_not_sent will be 1
            // 2) If the symbol has been sent before is_not_sent will be 0
            bool is_not_sent = !m_systematic_count.test(symbol_index);

            // Increment the count if the symbol was not previously sent
            m_systematic_count += is_not_sent;

            m_systematic_symbols_sent.set(symbol_index);
        }

        /// @return true if we are in the systematic phase (i.e. there
        /// are systematic packet to send) otherwise false
        bool in_systematic_phase() const
        {
            // We know that there must be some systematic symbols to
            // send if the rank (denotes the number of symbols
            // available in the encoder matrix is larger than the
            // number of systematic symbols we have sent
            return m_systematic_count < SuperCoder::rank();
        }

        /// @return The index of the next symbol to be sent in a
        uint32_t next_systematic_symbol() const
        {
            assert(is_systematic_phase());

            uint32_t next_symbol = 0;

            // Find which symbol should be the next to send systematically
            for(uint32_t i = m_offset; i < SuperCoder::symbols(); ++i)
            {
                bool is_not_sent = !m_systematic_symbols_sent.test(i);
                bool is_initialized = SuperCoder::is_symbol_initialized(i);

                if(is_not_sent && is_initialized)
                {
                    next_symbol = i;
                    break;
                }
            }

            // Update the offset when we can confirm that we have
            // systematically sent all symbols with a lower or equal
            // index than the next_symbol variable
            for(uint32_t i = m_offset; i <= next_symbol; ++i)
            {
                bool is_sent = m_systematic_count.test(i);
                bool is_initialized = SuperCoder::is_symbol_initialized(i);

                if(is_sent && is_symbol_initialized)
                {
                    ++m_offset;
                }
                else
                {
                    break;
                }
            }

            return next_symbol;

        }

    protected:

        /// When searching for the next symbol to send systematically
        /// we do not have to start from the beginning every time
        /// i.e. if we know that we have already sent the symbols with
        /// index 0 and 2 the we can start the search for the next
        /// symbol from index 1. The offset variable keeps track of
        /// the progress.
        uint32_t m_offset;

        /// Counts the number of systematic packets produced
        uint32_t m_systematic_count;

        /// Bitset which keeps track of the symbols that have been sent
        /// systematic
        boost::dynamic_bitset<uint8_t> m_systematic_symbols_sent;

    };

}

