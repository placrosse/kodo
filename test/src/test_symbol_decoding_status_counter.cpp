// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_symbol_decoding_status_counter.cpp Unit tests for the
///       kodo::symbol_decoding_status_counter

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/symbol_decoding_status_tracker.hpp>
#include <kodo/symbol_decoding_status_counter.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // symbol_decoding_status_counter layer.
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
        // symbol_decoding_status_counter layer.
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

        // Instantiate a stack containing the symbol_decoding_status_counter
        class test_stack
            : public // Payload API
                     // Codec Header API
                     // Symbol ID API
                     // Codec API
                     symbol_decoding_status_counter<
                     symbol_decoding_status_tracker<
                     // Coefficient Storage API
                     // Storage API
                     // Finite Field API
                     // Factory API
                     // Final type
                    dummy_layer> >
            { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestSymbolDecodingStatusCounter, api)
{
    kodo::test_stack stack;

    kodo::dummy_factory factory;
    factory.m_symbols = 10;

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.symbols(), factory.m_symbols);
    EXPECT_EQ(stack.symbols(), factory.symbols());

    EXPECT_EQ(stack.symbols_missing(), stack.symbols());
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    uint32_t symbol = 1;

    stack.set_symbol_seen(symbol);

    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 1U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    stack.set_symbol_uncoded(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 1U);

    stack.set_symbol_missing(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols());
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    // Go through the different transitions possible
    symbol = 9;

    // Missing -> Missing
    stack.set_symbol_missing(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols());
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    // Missing -> Seen
    stack.set_symbol_seen(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 1U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    // Seen -> Seen
    stack.set_symbol_seen(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 1U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    // Seen -> Decoded
    stack.set_symbol_uncoded(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 1U);

    // Decoded -> Decoded
    stack.set_symbol_uncoded(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 1U);

    // Decoded -> Seen
    stack.set_symbol_seen(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 1U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    // Seen -> Missing
    stack.set_symbol_missing(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols());
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

    // Missing -> Decoded
    stack.set_symbol_uncoded(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols() - 1);
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 1U);

    // Decoded -> Missing
    stack.set_symbol_missing(symbol);
    EXPECT_EQ(stack.symbols_missing(), stack.symbols());
    EXPECT_EQ(stack.symbols_seen(), 0U);
    EXPECT_EQ(stack.symbols_decoded(), 0U);

}


