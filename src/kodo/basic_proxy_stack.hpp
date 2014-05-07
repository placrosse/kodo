// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "basic_nested_stack.hpp"
#include "proxy_stack.hpp"

namespace kodo
{
    /// @todo test + docs
    template
    <
        class Args,
        template <class...> class NestedStack,
        class SuperCoder
    >
    using basic_proxy_stack = proxy_stack<
        basic_nested_stack, Args, NestedStack, SuperCoder>;


}
