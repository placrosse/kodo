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
    /// @todo add unit test
    /// @ingroup factory_layers
    ///
    /// Terminates the layered coder and contains the coder final
    /// factory. The pool factory uses a memory pool to recycle
    /// encoders/decoders, and thereby minimize memory consumption.
    template<class Codec>
    class pool_factory : public Codec::factory_base
    {
    public:

        /// Pointer type to the constructed coder
        typedef boost::shared_ptr<Codec> pointer;

    public:

        /// @copydoc layer::factory::factory(uint32_t,uint32_t)
        pool_factory(uint32_t max_symbols, uint32_t max_symbol_size) :
            Codec::factory_base(max_symbols, max_symbol_size),
            m_pool(std::bind(&pool_factory::make_coder, this))
        { }

        /// @copydoc layer::factory::build()
        pointer build()
        {
            pointer coder = m_pool.allocate();
            coder->initialize(*this);

            return coder;
        }

        /// @return A reference to the internal resource pool
        const sak::resource_pool<Codec>& pool() const
        {
            return m_pool;
        }

        /// @return A reference to the internal resource pool
        sak::resource_pool<Codec>& pool()
        {
            return m_pool;
        }

    public:

        /// @copydoc basic_factory::other<U>
        template<class U>
        using other = pool_factory<U>;

    private: // Make non-copyable

        /// Copy constructor
        pool_factory(const pool_factory&);

        /// Copy assignment
        const pool_factory& operator=(const pool_factory&);

    private:

        /// Factory function used by the resource pool to
        /// build new coders if needed.
        /// @param max_symbols The maximum symbols that are supported
        /// @param max_symbol_size The maximum size of a symbol in
        ///        bytes
        static pointer make_coder(pool_factory *factory)
        {
            assert(factory);

            pointer coder = boost::make_shared<Codec>();
            coder->construct(*factory);

            return coder;
        }

    private:

        /// Resource pool for the coders
        sak::resource_pool<Codec> m_pool;

    };
}
