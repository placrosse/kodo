// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{

    /// @todo group
    ///
    /// @brief Small helper class needed to disambiguate template
    ///        argument decuction when using a variadic template
    ///        argument _not_ as the last entry in the template
    ///        arguement list. @see the proxy_stack for the example
    ///        usage.
    template<class... Args>
    struct proxy_args
    { };

}

