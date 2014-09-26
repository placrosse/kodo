// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "has_construct.hpp"

namespace kodo
{
    /// @ingroup generic_api
    /// @copydoc construct(Stack&, Factory&)
    ///
    /// See the basic_factory for an example use-case. In those cases
    /// these helpers allow the factories to compile time detect
    /// whether the stack they build has the construct function or
    /// not.
    template
    <
        class Stack,
        class Factory,
        typename std::enable_if<has_construct<Stack>::value, uint8_t>::type = 0
    >
    inline void construct(Stack& stack, Factory& the_factory)
    {
        stack.construct(the_factory);
    }

    /// @ingroup generic_api
    /// @copydoc construct(Stack&, Factory&)
    template
    <
        class Stack,
        class Factory,
        typename std::enable_if<!has_construct<Stack>::value, uint8_t>::type = 0
    >
    inline void construct(Stack& stack, Factory& the_factory)
    {
        (void) stack;
        (void) the_factory;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // construct() function. However, this assert can
        // be avoided by using the has_construct
        assert(0);
    }
}
