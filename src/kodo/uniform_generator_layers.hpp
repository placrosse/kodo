// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "uniform_generator.hpp"
#include "seed_generator.hpp"

namespace kodo
{
    /// @ingroup coefficient_generator_layers
    ///
    /// @brief Template alias for the common set of uniform generator
    ///        layers used in most stacks
    template<class SuperCoder>
    using uniform_generator_layers =
        seed_generator<
        uniform_generator<SuperCoder>>;
}
