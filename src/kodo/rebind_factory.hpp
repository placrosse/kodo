// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @ingroup factory_layers
    ///
    /// @brief Helper function to make the rebind functionality of the
    ///        factories prettier. For an example see
    ///        basic_factory::other<U>.
    template<class From, class To>
    using rebind_factory = typename From::factory::template other<To>;
}
