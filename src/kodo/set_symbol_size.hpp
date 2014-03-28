// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_set_symbol_size.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @copydoc set_symbol_size(const T&, uint32_t)
    template
    <
        class T,
        typename std::enable_if<
            has_set_symbol_size<T>::value, uint8_t>::type = 0
    >
    inline void set_symbol_size(T& t, uint32_t symbol_size)
    {
        t.set_symbol_size(symbol_size);
    }

    /// @ingroup generic_api
    /// @copydoc set_symbol_size(const T&, uint32_t)
    template
    <
        class T,
        typename std::enable_if<
            !has_set_symbol_size<T>::value, uint8_t>::type = 0
    >
    inline void set_symbol_size(T& t, uint32_t symbol_size)
    {
        (void) t;
        (void) symbol_size;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // set_symbol_size() function. However, this assert can
        // be avoided by using the has_set_symbol_size
        assert(0);
    }

}


