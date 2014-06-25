// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <functional>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace kodo
{
    /// @ingroup factory
    ///
    /// @brief The basic factory implementation where a new heap
    ///        allocated codec is create every time the build()
    ///        function is invoked.
    template<class Codec>
    class basic_factory : public Codec::factory_base
    {
    public:

        /// Pointer type to the constructed coder
        typedef boost::shared_ptr<Codec> pointer;

    public:

        /// @copydoc layer::factory_base::factory(uint32_t,uint32_t)
        basic_factory(uint32_t max_symbols, uint32_t max_symbol_size) :
            Codec::factory_base(max_symbols, max_symbol_size)
        { }

        /// @copydoc factory::build()
        pointer build()
        {
            auto coder = boost::make_shared<Codec>();
            coder->construct(*this);
            coder->initialize(*this);

            return coder;
        }

    public:

        /// This type alias allows us to rebind the factory to build
        /// another type. This is useful in cases where we want to
        /// exend a stack but reuse the factory define in it. You can
        /// use the rebind_factory helper to make it look pretty :)
        ///
        /// Example:
        ///
        /// This example shows how an new layer can be wrapped around an
        /// existing stack. First lets make some dummy layer:
        ///
        ///    template<class SuperCoder>
        ///    class ping_layer : public SuperCoder
        ///    {
        ///    public:
        ///
        ///        void ping()
        ///        {
        ///            std::cout << "pong?" << std::endl;
        ///        }
        ///    };
        ///
        /// Now lets show how this layer could be added to an existing stack:
        ///
        ///    template<class Codec>
        ///    class wrap_ping : public ping_layer<Codec>
        ///    {
        ///    public:
        ///        using factory = rebind_factory<Codec, wrap_ping>;
        ///    };
        ///
        /// Usage:
        ///
        ///    wrap_ping<full_rlnc_decoder<binary>>::factory f(10,10);
        ///    auto p = f.build();
        ///
        ///  The wrap layer uses the rebind_helper here to use the
        ///  factory already define in full_rlnc_decoder. The
        ///  rebind_helper in turn uses the other template alias
        ///  define here.
        ///
        template<class U>
        using other = basic_factory<U>;

    private: // Make non-copyable

        /// Copy constructor
        basic_factory(const basic_factory&);

        /// Copy assignment
        const basic_factory& operator=(const basic_factory&);
    };
}
