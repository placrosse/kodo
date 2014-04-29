// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "shallow_symbol_storage.hpp"

namespace kodo
{
    /// @ingroup symbol_storage_layers
    /// @brief Defines a coding layer for 'const' symbol storage. Only
    /// useful for encoders since these to modify the buffers / data they
    /// operate on.
    template<class SuperCoder>
    using const_shallow_symbol_storage =
        shallow_symbol_storage<true, SuperCoder>;
}
