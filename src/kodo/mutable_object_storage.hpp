// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <sak/storage.hpp>

#include "object_storage.hpp"

namespace kodo
{
    /// @todo add docs
    template<class SuperCoder>
    using mutable_object_storage =
        object_storage<sak::mutable_storage, SuperCoder>;
}
