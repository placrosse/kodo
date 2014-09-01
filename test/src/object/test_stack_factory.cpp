// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_stack_factory.cpp Unit tests for the
///       stack_factory class

#include <cstdint>
#include <memory>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/object/stack_factory.hpp>

namespace kodo
{
// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    // This is the stack that our stack_factory layer will build
    class codec_stack
    {
    public:

        class factory
        {
        public:

            factory(uint32_t symbols, uint32_t symbol_size)
            {
                (void) symbols;
                (void) symbol_size;
            }

            std::shared_ptr<codec_stack> build()
            {
                auto codec = std::make_shared<codec_stack>();
            }

            void set_symbols(uint32_t symbols)
            {
                m_set_symbols(symbols);
            }

            void set_symbol_size(uint32_t symbol_size)
            {
                m_set_symbol_size(symbol_size);
            }

        public:

            stub::call<void(uint32_t)> m_set_symbols;
            stub::call<void(uint32_t)> m_set_symbol_size;
        };

    public:

        uint32_t symbols() const
        {
            return m_symbols();
        }

        uint32_t symbol_size() const
        {
            return m_symbol_size();
        }

    public:

        stub::call<uint32_t()> m_symbols;
        stub::call<uint32_t()> m_symbol_size;
    };

    /// Helper layer which will be the base class for the stack_factory layer
    class dummy_layer
    {
    public:

        class factory_base
        {
        public:

            factory_base(uint32_t symbols, uint32_t symbol_size)
            {
                (void) symbols;
                (void) symbol_size;
            }
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
            m_initialize();
        }

        uint32_t symbols(uint32_t index) const
        {
            return m_symbols(index);
        }

        uint32_t symbol_size(uint32_t index) const
        {
            return m_symbol_size(index);
        }

    public:

        stub::call<void()> m_initialize;
        stub::call<uint32_t(uint32_t)> m_symbols;
        stub::call<uint32_t(uint32_t)> m_symbol_size;

    };

    // Helper stack
    class dummy_stack : public object::stack_factory<codec_stack, dummy_layer>
    { };

    // Helper factory
    class dummy_factory
    { };
}
}

/// @todo figure out is this is the right naming
TEST(ObjectTestStackFactory, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 100;

    kodo::dummy_stack::factory_base factory(symbols, symbol_size);
    kodo::dummy_stack stack;

    stack.initialize(factory);
    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());

    stack.m_symbols.set_return(5);
    stack.m_symbol_size.set_return(10);

    auto codec = stack.build(0);




}
