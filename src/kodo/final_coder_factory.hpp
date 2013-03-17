// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef KODO_FINAL_CODER_FACTORY_HPP
#define KODO_FINAL_CODER_FACTORY_HPP

#include <stdint.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @todo remove the field from these classes - so we don't have to specify
    ///       a field when it does not make sense
    /// @ingroup factory_layers
    /// Terminates the layered coder and contains the coder allocation
    /// policy
    template<class FINAL, class Field>
    class final_coder_factory : boost::noncopyable
    {
    public:

        /// @copydoc layer::field_type
        typedef Field field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

        /// Pointer type to the constructed coder
        typedef boost::shared_ptr<FINAL> pointer;

        /// The final factory
        class factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : m_max_symbols(max_symbols),
                  m_max_symbol_size(max_symbol_size)
                {
                    assert(m_max_symbols > 0);
                    assert(m_max_symbol_size > 0);
                }

            /// @copydoc layer::factory::build(uint32_t,uint32_t)
            pointer build(uint32_t symbols, uint32_t symbol_size)
                {
                    assert(symbols > 0);
                    assert(symbol_size > 0);

                    pointer coder = boost::make_shared<FINAL>();

                    coder->construct(m_max_symbols, m_max_symbol_size);
                    coder->initialize(symbols, symbol_size);

                    return coder;
                }

        private:

            /// The maximum number of symbols
            uint32_t m_max_symbols;

            /// The maximum symbol size
            uint32_t m_max_symbol_size;
        };

    public:

        /// @copydoc layer::construct(uint32_t,uint32_t)
        void construct(uint32_t max_symbols, uint32_t max_symbol_size)
            {
                /// This is the final factory layer so we do nothing
                (void) max_symbols;
                (void) max_symbol_size;
            }

        /// @copydoc layer::initialize(uint32_t,uint32_t)
        void initialize(uint32_t symbols, uint32_t symbol_size)
            {
                /// This is the final factory layer so we do nothing
                (void) symbols;
                (void) symbol_size;
            }
    };
}

#endif

