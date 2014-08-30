// Copyright Steinwurf ApS 2011-2014.
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
    /// @todo docs
    template<class SuperCoder>
    using rfc5052_partitioning =
        partitioning<rfc5052_partitioning_scheme, SuperCoder>;
}
}
