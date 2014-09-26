// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "coefficient_value_access.hpp"
#include "coefficient_storage.hpp"
#include "coefficient_info.hpp"

namespace kodo
{
    /// @ingroup coefficient_storage_layers
    ///
    /// @brief Template alias for the common set of coefficient
    ///        storage layers used in most stacks
    template<class SuperCoder>
    using coefficient_storage_layers =
        coefficient_value_access<
        coefficient_storage<
        coefficient_info<SuperCoder> > >;
}
