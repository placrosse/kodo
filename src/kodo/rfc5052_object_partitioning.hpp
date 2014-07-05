// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "rfc5052_partitioning_scheme.hpp"
#include "object_partitioning.hpp"

namespace kodo
{
    /// @todo docs
    template<class SuperCoder>
    using rfc5052_object_partitioning =
        object_partitioning<rfc5052_partitioning_scheme, SuperCoder>;
}
