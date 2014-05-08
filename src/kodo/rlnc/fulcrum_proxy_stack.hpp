// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../proxy_stack.hpp"
#include "fulcrum_nested_stack.hpp"

namespace kodo
{
    /// @todo docs + test and find a way to avoid replicating
    ///       functionality between the standard proxy and nested
    ///       stacks and the fulcrum ones.
    template
    <
        class Args,
        template <class...> class NestedStack,
        class SuperCoder
    >
    using fulcrum_proxy_stack = proxy_stack<
        fulcrum_nested_stack, Args, NestedStack, SuperCoder>;
}
