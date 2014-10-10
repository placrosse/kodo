// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "nested_stack.hpp"
#include "basic_nested_stack_info.hpp"

namespace kodo
{
    /// @todo run regex for copyright year
    /// @todo docs + test
    template
    <
        class NestedStack,
        class SuperCoder
    >
    using basic_nested_stack =
        nested_stack<NestedStack,
        basic_nested_stack_info<SuperCoder> >;
}
