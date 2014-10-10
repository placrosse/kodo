// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "storage_block_size.hpp"
#include "storage_block_length.hpp"

namespace kodo
{
    /// @todo create test file
    /// @ingroup symbol_storage_layers
    ///
    /// @brief Template alias for commonly used storage block
    ///        information layers.
    template<class SuperCoder>
    using storage_block_info =
        storage_block_length<
        storage_block_size<SuperCoder>>;
}
