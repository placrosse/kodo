// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <functional>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <sak/resource_pool.hpp>

namespace kodo
{
    /// @ingroup factory_layers
    ///
    /// Terminates the layered coder and contains the coder final
    /// factory. The pool factory uses a memory pool to recycle
    /// encoders/decoders, and thereby minimize memory consumption.
    template<class Codec>
    class basic_factory : public Codec::factory_base
    {
    public:

        /// Pointer type to the constructed coder
        typedef boost::shared_ptr<Codec> pointer;

    public:

        /// @copydoc layer::factory::factory(uint32_t,uint32_t)
        basic_factory(uint32_t max_symbols, uint32_t max_symbol_size) :
            Codec::factory_base(max_symbols, max_symbol_size)
        { }

        /// @copydoc layer::factory::build()
        pointer build()
        {
            auto coder = boost::make_shared<Codec>();
            coder->construct(*this);
            coder->initialize(*this);

            return coder;
        }

    public:

        /// @todo docs
        template<class U>
        using other = basic_factory<U>;

    private: // Make non-copyable

        /// Copy constructor
        basic_factory(const basic_factory&);

        /// Copy assignment
        const basic_factory& operator=(const basic_factory&);
    };
}
