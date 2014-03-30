// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "nested_stack.hpp"

namespace kodo
{

    template<class... Args>
    struct proxy_args
    { };

    /// @todo The names are a bit confusing here since multiple things
    /// are called proxy stack
    ///
    /// The proxy stack layer supports advanced compositions of layers. It
    /// builds on the nested_stack layer which provides support for
    /// embedding a codec stack within a codec stack. In addition to the
    /// embedding provided by the nested_stack layer the proxy stack layer
    /// will forward all common calls not handled in the nested stack back
    /// to the container stack.
    ///
    /// A simple figure might be a better way of showing the functionality:
    ///
    ///                                      forwarding calls to nested proxy
    ///                                      stack
    ///
    ///                                  +---------------+
    ///            +--------------+      |               |
    ///            |   layer 0    |      |               v
    ///            +--------------+      |       +--------------+
    ///            |   layer 1    |+-----+       |   layer 0    |
    ///            |(proxy stack) |              +--------------+
    /// "main      |              |<-----+       |   layer 1    |    "proxy
    ///  stack"    +--------------+      |       +--------------+     stack"
    ///            |   layer 2    |      |       |   layer 2    |
    ///            +--------------+      |       +--------------+
    ///            |   layer 3    |      |       | proxy layer  |
    ///            +--------------+      |       +--------------+
    ///            |   layer 4    |      |               +
    ///            +--------------+      |               |
    ///                                  +---------------+
    ///
    ///                                  calls that are not implemented in the
    ///                                  proxy stack are forwarded back to the
    ///                                  "main" stack by the proxy layer
    ///
    /// Note, as shown in the figure the nested stack must use the proxy_layer
    /// to setup the proxy functionality (forwarding calls back to the main
    /// stack). If this functionality is not needed then the more simple
    /// nested_stack layer may be used instead.
    ///
    /// The proxy stack will setup the call forwarding by invoking the
    /// set_factory_proxy() and set_stack_proxy() functions in the
    /// nested stack.  In addition the proxy stack will assume that
    /// the proxy stack takes as first template argument the main
    /// stack type.
    ///
    /// Additional template arguments can be passed to the proxy_stack
    /// using the proxy_args helper class.
    ///
    /// Example use:
    ///
    ///
    template<class Args, template <class...> class ProxyStack, class SuperCoder>
    struct proxy_stack;

    /// Specialization of the proxy_stack allowing the use of the
    /// proxy_args helper to specify additional template arguments in
    /// the proxy stack
    template
    <
        class... Args,
        template <class...> class ProxyStack,
        class SuperCoder
    >
    class proxy_stack<proxy_args<Args...>, ProxyStack, SuperCoder> :
        public nested_stack<ProxyStack<SuperCoder, Args...>,SuperCoder>
    {
    public:

        /// Typedef of the "actual" SuperCoder type
        typedef nested_stack<ProxyStack<SuperCoder, Args...>,SuperCoder> Super;

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
                Super::factory::nested().set_factory_proxy(this);
            }

        };

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            Super::construct(the_factory);

            auto& nested_factory = the_factory.nested();
            nested_factory.set_stack_proxy(this);
        }

    };

}






