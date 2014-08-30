// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_set_symbols.hpp"

namespace kodo
{
    /// @ingroup generic_api
    /// @copydoc set_symbols(const T&, uint32_t)
    template
    <
        class T,
        typename std::enable_if<has_set_symbols<T>::value, uint8_t>::type = 0
    >
    inline void set_symbols(T& t, uint32_t symbols)
    {
        t.set_symbols(symbols);
    }

    /// @ingroup generic_api
    /// @copydoc set_symbols(const T&, uint32_t)
    template
    <
        class T,
        typename std::enable_if<!has_set_symbols<T>::value, uint8_t>::type = 0
    >
    inline void set_symbols(T& t, uint32_t symbols)
    {
        (void) t;
        (void) symbols;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // set_symbols() function. However, this assert can
        // be avoided by using the has_set_symbols
        assert(0);
    }

}
