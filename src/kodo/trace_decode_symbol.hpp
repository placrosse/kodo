// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>

#include <fifi/fifi_utils.hpp>

#include <sak/storage.hpp>

#include "trace.hpp"
#include "cache_decode_symbol.hpp"
#include "final_layer.hpp"
#include "coefficient_info.hpp"
#include "storage_block_size.hpp"
#include "storage_block_length.hpp"
#include "finite_field_info.hpp"
#include "disable_trace.hpp"
#include "enable_trace.hpp"
#include "hexdump.hpp"
#include "coefficient_value_access.hpp"
#include "basic_factory.hpp"

namespace kodo
{
    /// Fall-through case for the case where TraceTag is disable_trace
    template<class TraceTag, class SuperCoder>
    class trace_decode_symbol : public SuperCoder
    {
        static_assert(std::is_same<TraceTag, disable_trace>::value,
                      "Unexpected TraceTag should be disable_trace in the "
                      "fall-through case.");
    };

    /// @ingroup trace
    ///
    /// @brief This layer makes it easy to print the incoming coded symbol
    ///        and coding coefficients
    ///
    /// The layer supports printing both the coded symbol data through
    /// the print_cached_symbol_data() and the coding coefficients through the
    /// print_cached_symbol_coefficients().
    ///
    /// For the print_cached_symbol_data() function the output will be the
    /// value of the finite field elements stored in the symbol's data.
    ///
    /// For the print_cached_symbol_coefficients() there are two possible
    /// outputs:
    ///
    /// 1) For a coded symbol e.g. "C: 1 0 0" where C denotes that the symbol
    ///    is coded and the following is the numeric values of the coding
    ///    coefficients
    /// 2) For an uncoded symbol e.g. "U: symbol index = 4" in this case
    ///    U denotes that the symbol is uncoded and the symbol index denotes
    ///    which original source symbol the data represents.
    template<class SuperCoder>
    class trace_decode_symbol<enable_trace, SuperCoder> : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    protected:

        /// Helper stack using the cache_symbol_decode to store the
        /// symbol information for later processing
        class cache : public
            cache_decode_symbol<
            coefficient_value_access<
            coefficient_info<
            storage_block_length<
            storage_block_size<
            finite_field_info<field_type,
            final_layer> > > > > >
        {
        public:
            using factory = basic_factory<cache>;
        };

    public:

        /// @ingroup factory_base_layers
        ///
        /// The factory_base layer associated with this coder.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t, uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size),
                  m_cache(max_symbols, max_symbol_size)
            { }

        protected:

            /// Ensure that our layer can access the function defined
            /// in the factory
            friend class trace_decode_symbol;

            /// @return The factory associated with the helper cache object
            typename cache::factory& cache_factory()
            {
                return m_cache;
            }

        protected:

            /// Stores the factory of the helper cache objects
            typename cache::factory m_cache;

        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            auto& cache_factory = the_factory.cache_factory();
            cache_factory.set_symbols(the_factory.symbols());
            cache_factory.set_symbol_size(the_factory.symbol_size());

            if (!m_cache)
            {
                m_cache = cache_factory.build();
                assert(m_cache);
            }
            else
            {
                assert(m_cache);
                m_cache->initialize(cache_factory);
            }
        }


        /// @copydoc layer::trace(std::ostream&, const Filter&)
        template<class Filter>
        void trace(std::ostream& out, const Filter& filter)
        {
            assert(m_cache);

            if (m_cache->is_cache_valid())
            {
                if (filter("input_symbol_data"))
                {
                    out << "input_symbol_data:" << std::endl;
                    print_cached_symbol_data(out);
                    out << std::endl;
                }

                if (filter("input_symbol_coefficients"))
                {
                    out << "input_symbol_coefficients:" << std::endl;
                    print_cached_symbol_coefficients(out);
                    out << std::endl;
                }
            }

            // If the lower layers define the trace function forward it
            if (kodo::has_trace<SuperCoder>::value)
            {
                SuperCoder& next = *this;
                kodo::trace(next, out, filter);
            }
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            assert(m_cache);
            m_cache->decode_symbol(symbol_data, coefficients);

            SuperCoder::decode_symbol(symbol_data, coefficients);
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            assert(m_cache);
            m_cache->decode_symbol(symbol_data, symbol_index);

            SuperCoder::decode_symbol(symbol_data, symbol_index);
        }

        /// Prints the decoder's state to the output stream
        /// @param out, the output stream
        void print_cached_symbol_data(std::ostream &out) const
        {
            assert(m_cache);

            auto storage = sak::storage(m_cache->cached_symbol_data(),
                                        m_cache->symbol_size());

            hexdump hex(storage);
            hex.set_max_size(32);

            out << hex;
        }

        /// Prints the cached symbol coefficients to the output stream
        /// @param out, the output stream
        void print_cached_symbol_coefficients(std::ostream &out) const
        {
            assert(m_cache);

            if(m_cache->cached_symbol_coded())
            {
                out << "C: ";

                const value_type *c = reinterpret_cast<const value_type*>(
                    m_cache->cached_symbol_coefficients());

                for(uint32_t j = 0; j < m_cache->symbols(); ++j)
                {
                    value_type value = m_cache->coefficient_value(c, j);

                    static_assert(sizeof(uint32_t) >= sizeof(value_type),
                                  "value_type will overflow in this print");

                    out << (uint32_t) value << " ";

                }

                out << std::endl;
            }
            else
            {
                out << "U: symbol index = "
                    << m_cache->cached_symbol_index() << std::endl;
            }

        }

    private:

        /// Stores the cache helper object which stores the content of
        /// the data buffers passed to the decode_symbol(...)
        /// functions.
        typename cache::factory::pointer m_cache;

    };
}
