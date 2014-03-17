// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_rank_symbol_decoding_status_updater.cpp Unit tests for the
///       kodo::rank_symbol_decoding_status_updater

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/rank_symbol_decoding_status_updater.hpp>
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
        // rank_symbol_decoding_status_updater layer.
        struct dummy_layer
        {
            typedef uint32_t rank_type;

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_symbols = the_factory.symbols();
            }

            void decode(uint8_t *payload)
            {
                m_payload = payload;
            }

            uint32_t rank() const
            {
                return m_rank;
            }

            uint32_t remote_rank() const
            {
                return m_remote_rank;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint8_t* m_payload;
            uint32_t m_rank;
            uint32_t m_remote_rank;
            uint32_t m_symbols;
        };

        // Instantiate a stack containing the
        // rank_symbol_decoding_status_updater
        class dummy_stack
            : public // Payload API
                     rank_symbol_decoding_status_updater<
                     // Codec Header API
                     // Symbol ID API
                     // Decoder API
                     symbol_decoding_status_counter<
                     symbol_decoding_status_tracker<
                     // Coefficient Storage API
                     // Storage API
                     // Finite Field API
                     // Factory API
                     // Final type
                     dummy_layer> > >
        { };

        struct dummy_factory
        {
            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t max_symbols() const
            {
                return m_symbols;
            }

            uint32_t m_symbols;
        };
    }
}

TEST(TestRankSymbolDecodingStatusUpdator, api)
{
    kodo::dummy_factory factory;
    factory.m_symbols = 5;

    kodo::dummy_stack stack;
    stack.m_rank = 0;
    stack.m_remote_rank = 0;

    std::vector<uint8_t> payload(10);

    stack.construct(factory);
    stack.initialize(factory);
    EXPECT_EQ(stack.symbols(), factory.symbols());

    for(uint32_t i = 0; i < stack.symbols(); ++i)
        EXPECT_FALSE(stack.is_symbol_decoded(i));

    stack.m_rank = 1;
    stack.m_remote_rank = 2;

    stack.set_symbol_seen(1);

    stack.decode(&payload[0]);

    for(uint32_t i = 0; i < stack.symbols(); ++i)
        EXPECT_FALSE(stack.is_symbol_decoded(i));

    stack.m_rank = 2;
    stack.m_remote_rank = 2;

    stack.set_symbol_seen(0);

    stack.decode(&payload[0]);

    EXPECT_TRUE(stack.is_symbol_decoded(0));
    EXPECT_TRUE(stack.is_symbol_decoded(1));
    EXPECT_FALSE(stack.is_symbol_decoded(2));
    EXPECT_FALSE(stack.is_symbol_decoded(3));
    EXPECT_FALSE(stack.is_symbol_decoded(4));

    stack.m_rank = 3;
    stack.m_remote_rank = 3;

    stack.set_symbol_seen(4);

    stack.decode(&payload[0]);

    EXPECT_TRUE(stack.is_symbol_decoded(0));
    EXPECT_TRUE(stack.is_symbol_decoded(1));
    EXPECT_FALSE(stack.is_symbol_decoded(2));
    EXPECT_FALSE(stack.is_symbol_decoded(3));
    EXPECT_TRUE(stack.is_symbol_decoded(4));

    stack.m_rank = 3;
    stack.m_remote_rank = 5;

    stack.decode(&payload[0]);

    EXPECT_TRUE(stack.is_symbol_decoded(0));
    EXPECT_TRUE(stack.is_symbol_decoded(1));
    EXPECT_FALSE(stack.is_symbol_decoded(2));
    EXPECT_FALSE(stack.is_symbol_decoded(3));
    EXPECT_TRUE(stack.is_symbol_decoded(4));

    stack.m_rank = 4;
    stack.m_remote_rank = 5;

    stack.set_symbol_seen(2);

    stack.decode(&payload[0]);

    EXPECT_TRUE(stack.is_symbol_decoded(0));
    EXPECT_TRUE(stack.is_symbol_decoded(1));
    EXPECT_FALSE(stack.is_symbol_decoded(2));
    EXPECT_FALSE(stack.is_symbol_decoded(3));
    EXPECT_TRUE(stack.is_symbol_decoded(4));

    stack.m_rank = 5;
    stack.m_remote_rank = 5;

    stack.set_symbol_seen(3);

    stack.decode(&payload[0]);

    EXPECT_TRUE(stack.is_symbol_decoded(0));
    EXPECT_TRUE(stack.is_symbol_decoded(1));
    EXPECT_TRUE(stack.is_symbol_decoded(2));
    EXPECT_TRUE(stack.is_symbol_decoded(3));
    EXPECT_TRUE(stack.is_symbol_decoded(4));

}


