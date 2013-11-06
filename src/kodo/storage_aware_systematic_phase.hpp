// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <boost/dynamic_bitset.hpp>

namespace kodo
{

    /// @todo document this
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

            m_systematic_start = 0;
            m_systematic_symbols_sent.resize(the_factory.symbols(), false);
        }

        /// @copydoc layer::encode_symbol(uint8_t*,uint32_t)
        void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            asssert(symbol_index < m_systematic_symbols_sent.size());

            SuperCoder::encode_symbol(symbol_data, symbol_index);

            // If we have an one bit the symbol has been sent as systematic
            // so if the symbol has not been sent before is_not_sent will be 1
            // however if the symbol has been
            bool is_sent = !m_systematic_count.test(symbol_index);
            m_systematic_count += is_sent;

            m_systematic_count.reset(symbol_index);
        }

    protected:

        /// @return true if we are in the systematic phase (i.e. there
        /// are systematic packet to send) otherwise false
        bool in_systematic_phase() const
        {
            return m_systematic_count < SuperCoder::rank();
        }

        /// @return The index of the next symbol to be sent in a
        uint32_t next_systematic_symbol() const
        {

            /// @todo not correct yet but
            for(uint32_t i = m_systematic_start; i < SuperCoder::symbols(); ++i)
            {
                bool is_sent = !m_systematic_count.test(i);
                if(is_sent)
                {
                    m_systematic_start += 1;
                }
                else
                {
                    if(SuperCoder::is_symbol_initialized(i))
                    {
                        return i;
                    }
                }
            }
        }

    protected:

        /// Keeps track of which symbols that have been sent

        /// Counts the number of systematic packets produced
        uint32_t m_systematic_count;

        /// Bitset which keeps track of the symbols that have been sent
        /// systematic
        boost::dynamic_bitset<uint8_t> m_systematic_symbols_sent;

    };

}

