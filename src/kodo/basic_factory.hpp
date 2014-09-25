// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <functional>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "initialize.hpp"
#include "construct.hpp"

namespace kodo
{
    /// @ingroup factory_types
    ///
    /// @brief The basic factory implementation where a new heap
    ///        allocated codec is create every time the build()
    ///        function is invoked.
    ///
    /// The factory objects inherit from the factory_base type defined
    /// within the Codec. The purpose of this curiously looking
    /// approach is to all a Codec to "extend" the factory object with
    /// methods/state needed by the codec during construction and
    /// initialization.
    ///
    /// A small toy example:
    ///
    ///     class final
    ///     {
    ///     public:
    ///
    ///         class factory_base
    ///         { };
    ///     };
    ///
    ///     template<class SuperCoder>
    ///     class brewing_layer : public SuperCoder
    ///     {
    ///     public:
    ///
    ///         class factory_base : public SuperCoder::factory_base
    ///         {
    ///         public:
    ///
    ///             void set_number_of_cups(uint32_t cups)
    ///             { m_cups = cups; }
    ///
    ///             uint32_t number_of_cups() const
    ///             { return m_cups; }
    ///
    ///         private:
    ///             uint32_t m_cups;
    ///         };
    ///
    ///      public:
    ///
    ///          template<class Factory>
    ///          void initialize(Factory& the_factory)
    ///          {
    ///              m_cups = the_factory.number_of_cups();
    ///          }
    ///
    ///          uint32_t number_of_cups() const
    ///          { return m_cups; }
    ///
    ///       private:
    ///
    ///          uint32_t m_cups;
    ///     };
    ///
    ///     class brewer : public brewing_layer<final>
    ///     {
    ///     public:
    ///         using factory = basic_factory<brewer>;
    ///     };
    ///
    /// Usage:
    ///
    ///     brewer::factory factory;
    ///     factory.set_number_of_cups(3);
    ///
    ///     auto stack = factory.build();
    ///
    /// Notice how the factory functions needed by the brewing_layer
    /// are specified by extending factory_base in that layer. Once
    /// the basic_factory is specified it will use the factory_base to
    /// "adapt" it's functionality to support whatever is needed by
    /// the specific layers.
    template<class Codec>
    class basic_factory : public Codec::factory_base
    {
    public:

        /// Pointer type to the constructed coder
        typedef std::shared_ptr<Codec> pointer;

    public:

        /// @copydoc layer::factory_base::factory(uint32_t,uint32_t)
        basic_factory(uint32_t max_symbols, uint32_t max_symbol_size) :
            Codec::factory_base(max_symbols, max_symbol_size)
        { }

        /// @copydoc factory::build()
        pointer build()
        {
            auto codec = std::make_shared<Codec>();

            if (kodo::has_construct<Codec>::value)
            {
                kodo::construct(*codec, *this);
            }

            if (kodo::has_initialize<Codec>::value)
            {
                kodo::initialize(*codec, *this);
            }

            return codec;
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
