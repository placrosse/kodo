// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <fifi/default_field.hpp>

#include "finite_field_math.hpp"
#include "finite_field_info.hpp"

namespace kodo
{
    /// @todo docs
    /// @ingroup coefficient_storage_layers
    ///
    /// @brief Template alias for the common set of coefficient
    ///        storage layers used in most stacks
    template<class Field, class SuperCoder>
    using finite_field_layers =
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field, SuperCoder> >;
}
