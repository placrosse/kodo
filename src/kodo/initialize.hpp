// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_initialize.hpp"

namespace kodo
{
    /// @ingroup generic_api
    /// @copydoc initialize(Stack&, Factory&)
    ///
    /// See the basic_factory for an example use-case. In those cases
    /// these helpers allow the factories to compile time detect
    /// whether the stack they build has the initialize function or
    /// not.
    template
    <
        class Stack,
        class Factory,
        typename std::enable_if<has_initialize<Stack>::value, uint8_t>::type = 0
    >
    inline void initialize(Stack& stack, Factory& the_factory)
    {
        stack.initialize(the_factory);
    }

    /// @ingroup generic_api
    /// @copydoc initialize(Stack&, Factory&)
    template
    <
        class Stack,
        class Factory,
        typename std::enable_if<!has_initialize<Stack>::value, uint8_t>::type = 0
    >
    inline void initialize(Stack& stack, Factory& the_factory)
    {
        (void) stack;
        (void) the_factory;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // initialize() function. However, this assert can
        // be avoided by using the has_initialize
        assert(0);
    }
}
