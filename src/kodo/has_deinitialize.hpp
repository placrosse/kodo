// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @ingroup type_traits
    ///
    /// Type trait helper allows compile time detection of whether a
    /// codec contains a layer with the member function deinitialize
    ///
    /// Example:
    ///
    /// typedef kodo::full_rlnc8_encoder encoder_t;
    ///
    /// if(kodo::has_deinitialize<encoder_t>::value)
    /// {
    ///     // Do something here
    /// }
    ///
    template<class T>
    struct has_deinitialize
    {
    private:
        typedef std::true_type yes;
        typedef std::false_type no;

        // The deinitialize member function has the following prototype:
        // template<class Factory>
        // void deinitialize(Factory& the_factory)
        //
        // Because deinitialize takes a reference we cast 0 to a pointer
        // and dereference it to get a refence. The fact that we use
        // int as the type does not matter for the check.
        template<class U>
        static auto test(int) ->
            decltype(std::declval<U>().deinitialize(*(int*)0), yes());

        template<class> static no test(...);

    public:

        enum { value = std::is_same<decltype(test<T>(0)),yes>::value };
    };
}
