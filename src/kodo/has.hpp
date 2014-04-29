// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{

    /// @todo docs + test
    /// Type trait functionality for detecting whether
    template<typename> struct any_return { typedef void type; };

    template<typename B, template <class...> class D, typename Sfinae = void>
    struct has : public std::false_type {};

    // template<class T>
    // void has_helper(T *){}

    template<template <class...> class T, class... Args>
    void has_helper(T<Args...> *){}

    // template<template <class, class> class T, class Arg1, class Arg2>
    // void has_helper(T<Arg1, Arg2> *){}

    // template
    // <
    //     template <class, class, class> class T,
    //     class Arg1, class Arg2, class Arg3
    //     >
    // void has_helper(T<Arg1, Arg2, Arg3> *){}

    template<typename B, template <class...> class  D>
    struct has<B, D,
        typename any_return< decltype( has_helper<D>(static_cast<B*>( 0 )) ) >::type
        > : public std::true_type {};
}
