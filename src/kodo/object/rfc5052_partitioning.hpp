// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../rfc5052_partitioning_scheme.hpp"
#include "partitioning.hpp"

namespace kodo
{
namespace object
{
    /// @ingroup object_layers
    ///
    /// @brief Alias using the rfc5052 as the object partitioning scheme
    template<class SuperCoder>
    using rfc5052_partitioning =
        partitioning<rfc5052_partitioning_scheme, SuperCoder>;
}
}
