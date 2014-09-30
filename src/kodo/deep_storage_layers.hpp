// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "deep_symbol_storage.hpp"
#include "storage_bytes_used.hpp"
#include "storage_block_size.hpp"
#include "storage_block_length.hpp"
#include "trace_symbol_storage.hpp"

namespace kodo
{
    /// @ingroup symbol_storage_layers
    ///
    /// @brief Template alias for the common set of deep storage
    ///        layers used in most stacks which utilize deep storage
    template<class TraceTag, class SuperCoder>
    using deep_storage_layers =
        trace_symbol_storage<TraceTag,
        deep_symbol_storage<
        storage_bytes_used<
        storage_block_length<
        storage_block_size<SuperCoder> > > > >;
}
