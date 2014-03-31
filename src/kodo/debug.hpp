// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_debug.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @ingroup debug
    ///
    /// This function calls the layer::debug(std::ostream&) function
    /// used to produce a debug trace for the chosen codec.
    template
    <
        class T,
        typename std::enable_if<has_debug<T>::value, uint8_t>::type = 0
    >
    inline void debug(T& t, std::ostream& out)
    {
        t.debug(out);
    }

    /// @ingroup generic_api
    /// @ingroup debug
    /// @copydoc debug(const T&, std::ostream)
    template
    <
        class T,
        typename std::enable_if<!has_debug<T>::value, uint8_t>::type = 0
    >
    inline void debug(T& t, std::ostream& out)
    {
        (void) t;
        (void) out;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // debug() function. However, this assert can
        // be avoided by using the has_debug
        assert(0);
    }

    /// @ingroup generic_api
    /// @ingroup debug
    /// @copydoc debug(const T&, std::ostream&)
    template<class T>
    inline void debug(boost::shared_ptr<T>& t, std::ostream& out)
    {
        debug(*t, out);
    }

}


