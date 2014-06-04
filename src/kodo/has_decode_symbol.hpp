// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <iostream>

namespace kodo
{
    /// @ingroup type_traits
    ///
    /// Type trait helper allows compile time detection of whether a
    /// codec contains a layer with the two layer::decode_symbol(..)
    /// overloads.
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_encoder encoder_t;
    ///
    /// if (kodo::has_decode_symbol<encoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
    /// Note: that the both overloads of layer::decode_symbol() should be
    ///       available before value will be true
    template<typename T>
    struct has_decode_symbol
    {
    private:
        typedef std::true_type yes;
        typedef std::false_type no;

        template<typename U>
        static auto test(int) ->
            decltype(std::declval<U>().decode_symbol(nullptr,nullptr),
                     std::declval<U>().decode_symbol(nullptr, uint32_t(1)),
                     yes());

        template<typename> static no test(...);

    public:

        static const bool value = std::is_same<decltype(test<T>(0)),yes>::value;
    };
}
