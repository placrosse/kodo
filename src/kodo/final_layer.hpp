// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "pool_factory.hpp"

namespace kodo
{
    /// @ingroup factory_layers
    ///
    /// @brief Terminates the layered coder and contains the coder
    ///        allocation policy
    class final_layer
    {
    public:

        /// @ingroup factory_layers
        /// The final factory
        class factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
            {
                (void) max_symbols;
                (void) max_symbol_size;
            }
        };

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            /// This is the final factory layer so we do nothing
            (void) the_factory;
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            /// This is the final factory layer so we do nothing
            (void) the_factory;
        }
    };
}
