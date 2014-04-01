// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "coefficient_value_access.hpp"
#include "coefficient_storage.hpp"
#include "coefficient_info.hpp"

namespace kodo
{

    /// @todo documentation
    /// They implement the Coefficent Storage API
    template<class SuperCoder>
    using common_coefficient_storage_layers =
               coefficient_value_access<
               coefficient_storage<
               coefficient_info<SuperCoder> > >;

}



