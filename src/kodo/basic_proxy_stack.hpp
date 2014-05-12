// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "basic_nested_stack.hpp"
#include "proxy_stack.hpp"

namespace kodo
{
    /// @ingroup utility
    ///
    /// @brief The basic proxy stack creates a proxy stack based on
    ///        the basic_nested_stack layer.
    ///
    /// The proxy stack supports embedding a stack within a stack, see
    /// more details in the main proxy stack header
    /// (proxy_stack.hpp). The proxy stack builds on a nested stack,
    /// the basic proxy stack is a template alias for a common
    /// configuration namely where the basic_nested_layer is used to
    /// embed the stack.
    template
    <
        class Args,
        template <class...> class NestedStack,
        class SuperCoder
    >
    using basic_proxy_stack = proxy_stack<
        basic_nested_stack, Args, NestedStack, SuperCoder>;
}
