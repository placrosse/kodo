// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../nested_stack.hpp"
#include "fulcrum_nested_stack_info.hpp"

namespace kodo
{
    /// @todo docs + test
    template
    <
        class NestedStack,
        class SuperCoder
    >
    using fulcrum_nested_stack =
        nested_stack<NestedStack,
        fulcrum_nested_stack_info<SuperCoder> >;
}
