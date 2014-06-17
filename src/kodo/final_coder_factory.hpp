// Copyright Steinwurf ApS 2011-2012.
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
    /// Terminates the layered coder and contains the coder allocation
    /// policy
    template<class FinalType>
    class final_coder_factory
    {
    public:

        /// Pointer type to the constructed coder
        typedef boost::shared_ptr<FinalType> pointer;

        //typedef typename FinalType::factory factory_type;

        /// @ingroup factory_layers
        /// The final factory
        class factory_base
        {
        public:

            /// The factory_base type
            typedef typename FinalType::factory_base factory_type;

        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
            {
                (void) max_symbols;
                (void) max_symbol_size;
            }

            /// @copydoc layer::factory::build()
            pointer build()
            {
                factory_type *this_factory =
                    static_cast<factory_type*>(this);

                pointer coder = boost::make_shared<FinalType>();

                coder->construct(*this_factory);
                coder->initialize(*this_factory);

                return coder;
            }

        private: // Make non-copyable

            /// Copy constructor
            factory_base(const factory_base&);

            /// Copy assignment
            const factory_base& operator=(const factory_base&);

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

    protected:

        /// Constructor
        final_coder_factory()
        { }

        /// Destructor
        ~final_coder_factory()
        { }

    private: // Make non-copyable

        /// Copy constructor
        final_coder_factory(const final_coder_factory&);

        /// Copy assignment
        const final_coder_factory& operator=(
            const final_coder_factory&);
    };
}
