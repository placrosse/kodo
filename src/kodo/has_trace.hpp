// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <iostream>

namespace kodo
{
    /// @ingroup type_traits
    ///
    /// @ingroup trace
    ///
    /// Type trait helper allows compile time detection of whether a
    /// codec contains a layer with the member function
    /// layer::trace(std::ostream&, const Filter&)
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_encoder encoder_t;
    ///
    /// if (kodo::has_trace<encoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
    template<typename T>
    struct has_trace
    {
    private:
        typedef std::true_type yes;
        typedef std::false_type no;

        // Since the trace function's filter parameter is a template
        // argument we don't have any way here of checking the type of
        // filter so we just pass zero
        template<typename U>
        static auto test(int) ->
            decltype(std::declval<U>().trace(std::cout, 0), yes());

        template<typename> static no test(...);

    public:

        static const bool value = std::is_same<decltype(test<T>(0)),yes>::value;
    };
}
