// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <functional>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <sak/resource_pool.hpp>

#include "initialize.hpp"
#include "construct.hpp"

namespace kodo
{
    /// @ingroup factory_types
    ///
    /// @brief The pool factory uses a memory pool to recycle
    ///         encoders/decoders, and thereby minimize memory
    ///         consumption. The current memory pool is not
    ///         thread-safe and therefore should not be used in
    ///         multi-threaded environments.
    ///
    /// For further descriptions of the factory pattern use in Kodo
    /// can be found in basic_factory.hpp
    template<class Codec>
    class pool_factory : public Codec::factory_base
    {
    public:

        /// Pointer type to the constructed coder
        using pointer = typename sak::resource_pool<Codec>::value_ptr;

    public:

        /// @copydoc layer::factory_base::factory(uint32_t,uint32_t)
        pool_factory(uint32_t max_symbols, uint32_t max_symbol_size) :
            Codec::factory_base(max_symbols, max_symbol_size),
            m_pool(std::bind(&pool_factory::make_codec, this))
        { }

        /// @copydoc factory::build()
        pointer build()
        {
            auto codec = m_pool.allocate();

            if (kodo::has_initialize<Codec>::value)
            {
                kodo::initialize(*codec, *this);
            }

            return codec;
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
        /// build new codecs if needed.
        /// @param factory Pointer to the factory object
        static pointer make_codec(pool_factory *factory)
        {
            assert(factory);

            pointer codec = std::make_shared<Codec>();

            if (kodo::has_construct<Codec>::value)
            {
                kodo::construct(*codec, *factory);
            }

            return codec;
        }

    private:

        /// Resource pool for the codecs
        sak::resource_pool<Codec> m_pool;
    };
}
