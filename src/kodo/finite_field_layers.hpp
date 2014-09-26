// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <fifi/default_field.hpp>

#include "finite_field_math.hpp"
#include "finite_field_info.hpp"

namespace kodo
{
    /// @ingroup finite_field_layers
    ///
    /// @brief Template alias for the common set of finite field
    ///        layers used in most stacks
    template<class Field, class SuperCoder>
    using finite_field_layers =
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field, SuperCoder> >;
}
