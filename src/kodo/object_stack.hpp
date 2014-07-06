// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @todo docs
    ///
    /// @brief Defines the stack type to use within the object stack.
    ///
    ///
    template<class Stack, class SuperCoder>
    class object_stack : public SuperCoder
    {
    public:

        //
        using stack_type = Stack;
    };
}
