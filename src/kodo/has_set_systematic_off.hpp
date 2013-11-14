// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "systematic_encoder.hpp"

namespace kodo
{

    /// @ingroup type_traits
    /// Type trait helper allows compile time detection of whether an
    /// encoder contains a layer with the member function set_systematic_off()
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_encoder encoder_t;
    ///
    /// if(kodo::has_set_systematic_off<encoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
template<typename T>
struct has_set_systematic_off
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;

	template<typename U> static auto test(int) -> decltype(std::declval<U>().set_systematic_off(), yes());

	template<typename> static no test(...);

public:

	static constexpr bool value = std::is_same<decltype(test<T>(0)),yes>::value;
};

    // template <typename T>
    // class has_set_systematic_off
    // {
    // private:

    //     template <typename U, U>
    //     class check
    //     { };

    //     template <typename C>
    //     static char f(check<void (C::*)(), &C::set_systematic_off>*);

    //     template <typename C>
    //     static long f(...);

    // public:
    //     static const bool value = (sizeof(f<T>(0)) == sizeof(char));
    // };

    // template<typename T, typename Sfinae = void>
    // struct has_set_systematic_off :
    //     std::false_type {};

    // template<typename T>
    // struct has_set_systematic_off<T,
    //     decltype( std::declval<T>().set_systematic_off() )> :
    //     std::true_type {};

}


