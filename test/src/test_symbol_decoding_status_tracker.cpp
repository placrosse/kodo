// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_symbol_decoding_status_tracker.cpp Unit tests for the
///       kodo::symbol_decoding_status_tracker

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/symbol_decoding_status_tracker.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {


        // Small helper struct which provides the API needed by the
        // symbol_decoding_status_tracker layer.
        struct dummy_layer
        {

            uint32_t symbols() const
            {
                return m_symbols;
            }

            template<class Factory>
            void construct(Factory &the_factory)
            {
                (void) the_factory;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_symbols = the_factory.symbols();
            }

            uint32_t m_symbols;
        };

        // Small helper struct which provides the API needed by the
        // symbol_decoding_status_tracker layer.
        struct dummy_factory
        {

            uint32_t max_symbols() const
            {
                return m_symbols;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t m_symbols;
        };



        // Instantiate a stack containing the symbol_decoding_status_tracker
        class test_stack
            : public
              // Payload API
              // Codec Header API
              // Symbol ID API
              // Codec API
              symbol_decoding_status_tracker<
              // Coefficient Storage API
              // Storage API
              // Finite Field API
              // Final Layer
              // Final type
              dummy_layer>
          { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestSymbolDecodingStatusTracker, api)
{
    kodo::test_stack stack;

    kodo::dummy_factory factory;
    factory.m_symbols = 10;

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.symbols(), factory.m_symbols);
    EXPECT_EQ(stack.symbols(), factory.symbols());

    // Initially all symbols should be missing
    for(uint32_t i = 0; i < stack.symbols(); ++i)
    {
        EXPECT_TRUE(stack.is_symbol_missing(i));
        EXPECT_FALSE(stack.is_symbol_seen(i));
        EXPECT_FALSE(stack.is_symbol_uncoded(i));
    }

    uint32_t symbol = 1;

    stack.set_symbol_seen(symbol);
    EXPECT_FALSE(stack.is_symbol_missing(symbol));
    EXPECT_TRUE(stack.is_symbol_seen(symbol));
    EXPECT_FALSE(stack.is_symbol_uncoded(symbol));

    stack.set_symbol_uncoded(symbol);
    EXPECT_FALSE(stack.is_symbol_missing(symbol));
    EXPECT_FALSE(stack.is_symbol_seen(symbol));
    EXPECT_TRUE(stack.is_symbol_uncoded(symbol));

    stack.set_symbol_missing(symbol);
    EXPECT_TRUE(stack.is_symbol_missing(symbol));
    EXPECT_FALSE(stack.is_symbol_seen(symbol));
    EXPECT_FALSE(stack.is_symbol_uncoded(symbol));

    // Again all symbols should be missing
    for(uint32_t i = 0; i < stack.symbols(); ++i)
    {
        EXPECT_TRUE(stack.is_symbol_missing(i));
        EXPECT_FALSE(stack.is_symbol_seen(i));
        EXPECT_FALSE(stack.is_symbol_uncoded(i));
    }

    symbol = 9;

    stack.set_symbol_seen(symbol);
    EXPECT_FALSE(stack.is_symbol_missing(symbol));
    EXPECT_TRUE(stack.is_symbol_seen(symbol));
    EXPECT_FALSE(stack.is_symbol_uncoded(symbol));

    stack.set_symbol_uncoded(symbol);
    EXPECT_FALSE(stack.is_symbol_missing(symbol));
    EXPECT_FALSE(stack.is_symbol_seen(symbol));
    EXPECT_TRUE(stack.is_symbol_uncoded(symbol));

    stack.set_symbol_missing(symbol);
    EXPECT_TRUE(stack.is_symbol_missing(symbol));
    EXPECT_FALSE(stack.is_symbol_seen(symbol));
    EXPECT_FALSE(stack.is_symbol_uncoded(symbol));


}
