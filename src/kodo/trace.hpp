// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_trace.hpp"

namespace kodo
{

    /// @ingroup generic_api
    /// @ingroup trace
    ///
    /// This function calls the layer::trace(std::ostream&) function
    /// used to produce a trace trace for the chosen codec.
    template
    <
        class T,
        typename std::enable_if<has_trace<T>::value, uint8_t>::type = 0
    >
    inline void trace(T& t, std::ostream& out)
    {
        t.trace(out);
    }

    /// @ingroup generic_api
    /// @ingroup trace
    /// @copydoc trace(const T&, std::ostream)
    template
    <
        class T,
        typename std::enable_if<!has_trace<T>::value, uint8_t>::type = 0
    >
    inline void trace(T& t, std::ostream& out)
    {
        (void) t;
        (void) out;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // trace() function. However, this assert can
        // be avoided by using the has_trace
        assert(0);
    }

    /// @ingroup generic_api
    /// @ingroup trace
    /// @copydoc trace(const T&, std::ostream&)
    template<class T>
    inline void trace(boost::shared_ptr<T>& t, std::ostream& out)
    {
        trace(*t, out);
    }

}
