// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_deep_symbol_storage.cpp Unit tests for the
///       deep_symbol_storage class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/deep_symbol_storage.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        /// Helper layer
        class dummy_layer
        {
        public:

            using value_type = uint8_t;

        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
                m_initialize();
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
                m_initialize();
            }

            uint32_t symbols() const
            {
                return m_symbols();
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size();
            }


            stub::call<void()> m_initialize;

            stub::call<uint32_t()> m_symbols;
            stub::call<uint32_t()> m_symbol_size;
        };

        // Helper stack
        class dummy_stack : public deep_symbol_storage<dummy_layer>
        { };

        // Helper factory
        class dummy_factory
        {
        public:

            uint32_t max_symbols() const
            {
                return m_max_symbols();
            }

            uint32_t max_symbol_size() const
            {
                return m_max_symbol_size();
            }

            stub::call<uint32_t()> m_max_symbols;
            stub::call<uint32_t()> m_max_symbol_size;
        };
    }
}

TEST(TestDeepSymbolStorage, api)
{
    kodo::dummy_factory factory;

    factory.m_max_symbols.set_return(10U);
    factory.m_max_symbol_size.set_return(100U);

    kodo::dummy_stack stack;
    stack.m_symbols.set_return(10U);
    stack.m_symbol_size.set_return(100U);




    // stack.m_block_size.set_return(10U);

    // stack.initialize(factory);
    // stack.set_bytes_used(9U);

    // EXPECT_EQ(stack.m_initialize.calls(), 1U);
    // EXPECT_TRUE(stack.m_block_size.called_once_with());
    // EXPECT_EQ(stack.bytes_used(), 9U);

    // // Initialize again and check that the state is reset
    // stack.initialize(factory);
    // EXPECT_EQ(stack.m_initialize.calls(), 2U);
    // EXPECT_TRUE(stack.m_block_size.called_once_with());
    // EXPECT_EQ(stack.bytes_used(), 0U);

}
