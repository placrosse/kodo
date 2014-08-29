// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_restore_partial_symbol_decoder.cpp Unit tests for the
///       restore_partial_symbol_decoder class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/restore_partial_symbol_decoder.hpp>
#include <kodo/wrap_restore_partial_symbol_decoder.hpp>

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

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
                m_initialize();
            }

            void decode(uint8_t *payload)
            {
                m_decode(payload);
            }

            bool is_complete()
            {
                return m_is_complete();
            }

            bool has_partial_symbol()
            {
                return m_has_partial_symbol();
            }

            void restore_partial_symbol()
            {
                m_restore_partial_symbol();
            }

            stub::call<void()> m_initialize;
            stub::call<void(uint8_t*)> m_decode;
            stub::call<bool()> m_is_complete;
            stub::call<bool()> m_has_partial_symbol;
            stub::call<void()> m_restore_partial_symbol;
        };

        // Helper stack
        class dummy_stack : public restore_partial_symbol_decoder<dummy_layer>
        { };

        // Helper factory
        class dummy_factory
        { };


    }
}

/// Tests cases where the restore_partial_symbol should not be called
TEST(TestRestorePartialSymbolDecoder, no_restore)
{
    kodo::dummy_factory factory;
    kodo::dummy_stack stack;

    stack.initialize(factory);

    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());

    stack.m_is_complete.set_return(false);
    stack.m_has_partial_symbol.set_return(false);

    // dummy variables
    uint8_t* pointer = (uint8_t*)0xdeadbeef;

    stack.decode(pointer);
    EXPECT_EQ(stack.m_restore_partial_symbol.calls(), 0U);
    EXPECT_EQ(stack.m_decode.calls(), 1U);

    stack.m_is_complete.set_return(true);

    stack.decode(pointer);
    EXPECT_EQ(stack.m_restore_partial_symbol.calls(), 0U);
    EXPECT_EQ(stack.m_decode.calls(), 2U);
}

/// Tests cases where the restore_partial_symbol should be called
TEST(TestRestorePartialSymbolDecoder, restore)
{
    kodo::dummy_factory factory;
    kodo::dummy_stack stack;

    stack.initialize(factory);

    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());

    stack.m_is_complete.set_return(false);
    stack.m_has_partial_symbol.set_return(true);

    // dummy variables
    uint8_t* pointer = (uint8_t*)0xdeadbeef;

    stack.decode(pointer);
    EXPECT_EQ(stack.m_restore_partial_symbol.calls(), 0U);
    EXPECT_EQ(stack.m_decode.calls(), 1U);

    stack.m_is_complete.set_return(true);

    stack.decode(pointer);
    EXPECT_EQ(stack.m_restore_partial_symbol.calls(), 1U);
    EXPECT_EQ(stack.m_decode.calls(), 2U);

    // We initialize again, the stack should restore on the first call
    stack.initialize(factory);

    stack.decode(pointer);
    EXPECT_EQ(stack.m_restore_partial_symbol.calls(), 2U);
    EXPECT_EQ(stack.m_decode.calls(), 3U);
}



/// Test that we can wrap an stack with the
/// wrap_restore_partial_symbol_decoder layer
TEST(TestRestorePartialSymbolDecoder, wrap)
{
    /// @todo missing
}
