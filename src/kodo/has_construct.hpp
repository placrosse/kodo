// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @ingroup type_traits
    ///
    /// Type trait helper allows compile time detection of whether a
    /// codec contains a layer with the member function construct
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_encoder encoder_t;
    ///
    /// if(kodo::has_construct<encoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
    template<class T>
    struct has_construct
    {
    private:
        typedef std::true_type yes;
        typedef std::false_type no;

        // The construct member function has the following prototype:
        // template<class Factory>
        // void construct(Factory& the_factory)
        //
        // Because construct takes a reference we cast 0 to a pointer
        // and dereference it to get a refence. The fact that we use
        // int as the type does not matter for the check.
        template<class U>
        static auto test(int) ->
            decltype(std::declval<U>().construct(*(int*)0), yes());

        template<class> static no test(...);

    public:

        enum { value = std::is_same<decltype(test<T>(0)),yes>::value };
    };
}
