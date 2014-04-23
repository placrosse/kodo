// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>

#include <fifi/fifi_utils.hpp>

#include <sak/storage.hpp>

#include "trace.hpp"
#include "cached_symbol_decoder.hpp"
#include "final_coder_factory.hpp"
#include "coefficient_info.hpp"
#include "storage_block_info.hpp"
#include "finite_field_info.hpp"

namespace kodo
{

    /// @todo Implement debug() function

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
    class trace_decode_symbol : public SuperCoder
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
            cached_symbol_decoder<
            coefficient_info<
            storage_block_info<
            finite_field_info<field_type,
            final_coder_factory<cache> > > > > { };

    public:

        /// @ingroup factory_layers
        ///
        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t, uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size),
                  m_cache(max_symbols, max_symbol_size)
            { }

        protected:

            friend class trace_decode_symbol;

            /// @todo docs
            typename cache::factory& cache_factory()
            {
                return m_cache;
            }

        protected:

            /// @todo docs
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

        /// @copydoc layer::trace(std::ostream&)
        void trace(std::ostream& out)
        {
            print_cached_symbol_data(out);

            // If the lower layers define the trace function forward it
            if (kodo::has_trace<SuperCoder>::value)
            {
                SuperCoder& next = *this;
                kodo::trace(next, out);
            }
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            assert(m_cache);
            // m_cache->decode_symbol(symbol_data, coefficients);

            SuperCoder::decode_symbol(symbol_data, coefficients);
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            assert(m_cache);
            // m_cache->decode_symbol(symbol_data, symbol_index);

            SuperCoder::decode_symbol(symbol_data, symbol_index);
        }


        /// Prints the decoder's state to the output stream
        /// @param out, the output stream
        void print_cached_symbol_data(std::ostream &out) const
        {
            // We calculate the number of finite field elements that are
            // stored in the symbols' data. In this case layer::symbol_length()
            // does not yield the correct result for fields where multiple
            // field elements are packed into the same value_type
            uint32_t symbol_elements =
                fifi::size_to_elements<field_type>(
                    SuperCoder::symbol_size());

            const value_type *s = reinterpret_cast<const value_type*>(
                SuperCoder::cached_symbol_data());

            for(uint32_t j = 0; j < symbol_elements; ++j)
            {
                value_type value = SuperCoder::coefficient_value(s, j);

                static_assert(sizeof(uint32_t) >= sizeof(value_type),
                              "value_type will overflow in this print");

                out << (uint32_t) value << " ";
            }

            out << std::endl;

        }

        /// Prints the cached symbol coefficients to the output stream
        /// @param out, the output stream
        void print_cached_symbol_coefficients(std::ostream &out) const
        {
            if(SuperCoder::cached_symbol_coded())
            {
                out << "C:  ";

                const value_type *c = reinterpret_cast<const value_type*>(
                    SuperCoder::cached_symbol_coefficients());

                for(uint32_t j = 0; j < SuperCoder::symbols(); ++j)
                {
                    value_type value = SuperCoder::coefficient_value(c, j);

                    static_assert(sizeof(uint32_t) >= sizeof(value_type),
                                  "value_type will overflow in this print");

                    out << (uint32_t) value << " ";

                }
            }
            else
            {
                out << "U:  symbol index = "
                    << SuperCoder::cached_symbol_index() << std::endl;
            }

            out << std::endl;
        }

    private:

        /// @todo docs
        typename cache::pointer m_cache;

    };

}
