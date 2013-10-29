// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <boost/dynamic_bitset.hpp>

namespace kodo
{

    template<class SuperCoder>
    class storage_aware_systematic_encoder : public SuperCoder
    {

    protected:

        typedef typename SuperCoder::status_vector status_vector;

    public:

        /// @copdydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_systematic_symbols.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_systematic_symbols.reset();
        }

        /// @copydoc layer::encode(uint8_t*, uint8_t*)
        uint32_t encode(uint8_t *symbol_data, uint8_t *symbol_header)
        {
            assert(symbol_data != 0);
            assert(symbol_header != 0);

            size_t next_symbol;
            size_t systematic_count = m_systematic_symbols.count();
            size_t symbols_initialized = SuperCoder::rank();
            bool in_systematic_phase = systematic_count < symbols_initialized;

            if(!SuperCoder::is_systematic_on() || !in_systematic_phase)
            {
                return SuperCoder::encode_non_systematic(symbol_data,
                                                         symbol_header);
            }

            next_symbol = next_available_symbol();
            m_systematic_symbols[next_symbol] = true;

            return SuperCoder::encode_systematic(symbol_data, symbol_header,
                                                 next_symbol);
        }

    private:

        uint32_t next_available_symbol()
        {
            status_vector available;

            // Mask out the out symbols that have already been sent
            // systematically from the set of symbols in the storage layer.
            available = ~m_systematic_symbols & SuperCoder::storage_status();

            return available.find_first();
        }

        status_vector m_systematic_symbols;

    };

}

