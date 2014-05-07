// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "proxy_args.hpp"

namespace kodo
{
    /// @ingroup utility
    ///
    /// @brief The proxy stack layer supports advanced compositions of
    ///        layers. It builds on the nested_stack layer which
    ///        provides support for embedding a codec stack within a
    ///        codec stack.
    ///
    /// In addition to the embedding provided by the nested_stack
    /// layer the proxy_layer will forward all common calls not
    /// handled in the nested stack back to the container stack.
    ///
    /// A simple figure might be a better way of showing the functionality:
    ///
    ///                                      forwarding calls to nested
    ///                                      stack
    ///
    ///                                  +---------------+
    ///            +--------------+      |               |
    ///            |   layer 0    |      |               v
    ///            +--------------+      |       +--------------+
    ///            |   layer 1    |+-----+       |   layer 0    |
    ///            |(proxy_stack) |              +--------------+
    /// "main      |              |<-----+       |   layer 1    |    "nested
    ///  stack"    +--------------+      |       +--------------+     stack"
    ///            |   layer 2    |      |       |   layer 2    |
    ///            +--------------+      |       +--------------+
    ///            |   layer 3    |      |       | proxy_layer  |
    ///            +--------------+      |       +--------------+
    ///            |   layer 4    |      |               +
    ///            +--------------+      |               |
    ///                                  +---------------+
    ///
    ///                                  calls that are not implemented in the
    ///                                  nested stack are forwarded back to the
    ///                                  "main" stack by the proxy_layer
    ///
    /// Note, as shown in the figure the nested stack must use the proxy_layer
    /// to setup the proxy functionality (forwarding calls back to the main
    /// stack). If this functionality is not needed then the more simple
    /// nested_stack layer may be used instead.
    ///
    /// The proxy_stack will setup the call forwarding by invoking the
    /// set_main_factory() and set_main_stack() functions in the
    /// nested stack.  In addition the proxy_stack will assume that
    /// the nested stack takes as first template argument the main
    /// stack type.
    ///
    /// Additional template arguments can be passed to the nested stack
    /// using the proxy_args helper class.
    ///
    /// Example use:
    ///
    ///     // A "nested stack" with the proxy_layer at the bottom
    ///     template<class MainStack>
    ///     class my_nested_stack : public
    ///         computation_layer_one<
    ///         computation_layer_two<
    ///         proxy_layer<my_nested_stack<MainStack>, MainStack> > >
    ///     { };
    ///
    ///     // The "main stack" using the "nested stack"
    ///     class main_stack : public
    ///         some_layer_one<
    ///         proxy_stack<proxy_args<>, my_nested_stack,
    ///         some_layer_two<
    ///         some_layer_three> > >
    ///     { };
    ///
    ///
    /// Now imagine if the my_nested_stack took an additional template
    /// argument. If that was the case we would use the proxy_args<..>
    /// helper to pass them in.
    ///
    /// Example:
    ///
    ///     template<class Another, class MainStack>
    ///     class my_nested_stack : public
    ///         ...
    ///
    ///     class main_stack : public
    ///         some_layer_one<
    ///         proxy_stack<proxy_args<int>, my_nested_stack,
    ///
    /// This code would pass int as the Another template argument - cool :)
    ///
    template
    <
        template <class...> class NestedImpl,
        class Args,
        template <class...> class NestedStack,
        class SuperCoder
    >
    struct proxy_stack;

    /// Specialization of the proxy_stack allowing the use of the
    /// proxy_args helper to specify additional template arguments in
    /// the proxy stack
    template
    <
        template <class...> class NestedImpl,
        class... Args,
        template <class...> class NestedStack,
        class SuperCoder
    >
    class proxy_stack<NestedImpl, proxy_args<Args...>, NestedStack, SuperCoder> :
        public NestedImpl<NestedStack<SuperCoder, Args...>,SuperCoder>
    {
    public:

        /// Typedef of the "actual" SuperCoder type
        typedef NestedImpl<NestedStack<SuperCoder, Args...>,SuperCoder> Super;

    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory : public Super::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size) :
                Super::factory(max_symbols, max_symbol_size)
            {
                Super::factory::nested().set_main_factory(this);
            }

        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            // Here perform a bit of work before calling
            // Super::initialize(...) which is not typical. In this
            // case we have to make sure the nested factory is in the
            // correct state - in general the user (i.e you ;)) are
            // the one responsible for configuring the state of a
            // factory before building a new coder. However, in this
            // case the nested/proxy stack is held withing the main
            // stack we therefore have to set the state.
            auto& nested_factory = the_factory.nested();

            assert(nested_factory.main_factory() == &the_factory);

            nested_factory.set_main_stack(this);

            Super::initialize(the_factory);

            assert(Super::nested()->main_stack() == this);

        }

    };
}
