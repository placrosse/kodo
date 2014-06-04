// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_storage_aware_systematic_phase.cpp Unit tests for the
///       kodo::storage_aware_systematic_phase layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/storage_aware_systematic_phase.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // storage_aware_systematic_phase layer.
        struct dummy_layer
        {

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_symbols = the_factory.symbols();
                m_is_symbol_uncoded.resize(m_symbols, false);
            }

            void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
            {
                (void) symbol_data;
                (void) symbol_index;
            }

            uint32_t symbols_uncoded() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_is_symbol_uncoded[i];
                }
                return rank;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            bool is_symbol_uncoded(uint32_t index) const
            {
                (void) index;
                return m_is_symbol_uncoded[index];
            }

            uint32_t m_symbols;
            std::vector<bool> m_is_symbol_uncoded;
        };

        // Instantiate a stack containing the storage_aware_systematic_phase
        class dummy_stack
            : public storage_aware_systematic_phase<
                     dummy_layer>
          { };

        class dummy_factory
        {
        public:

            uint32_t max_symbols() const
            {
                return m_max_symbols;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t m_max_symbols;
            uint32_t m_symbols;
        };
    }
}

/// Run the tests
TEST(TestStorageAwareSystematicPhase, api)
{
    kodo::dummy_stack stack;
    kodo::dummy_factory factory;

    factory.m_max_symbols = 8;
    factory.m_symbols = 8;

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.systematic_count(), 0U);

    stack.m_is_symbol_uncoded[0] = true;

    EXPECT_EQ(stack.symbols(), factory.symbols());
    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 0U);

    uint8_t* symbol = 0;

    EXPECT_EQ(stack.next_systematic_symbol(), 0U);

    stack.encode_symbol(symbol, 0);

    EXPECT_EQ(stack.in_systematic_phase(), false);
    EXPECT_EQ(stack.systematic_count(), 1U);

    stack.m_is_symbol_uncoded[2] = true;
    stack.m_is_symbol_uncoded[3] = true;

    EXPECT_EQ(stack.symbols_uncoded(), 3U);
    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 2U);

    // We resend the first symbol nothing should change
    stack.encode_symbol(symbol, 0);

    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 2U);
    EXPECT_EQ(stack.systematic_count(), 1U);

    stack.encode_symbol(symbol, 2);

    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 3U);
    EXPECT_EQ(stack.systematic_count(), 2U);

    stack.encode_symbol(symbol, 3);

    EXPECT_EQ(stack.in_systematic_phase(), false);
    EXPECT_EQ(stack.systematic_count(), 3U);

    stack.m_is_symbol_uncoded[1] = true;

    EXPECT_EQ(stack.symbols_uncoded(), 4U);
    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 1U);
    EXPECT_EQ(stack.systematic_count(), 3U);

    stack.encode_symbol(symbol, 1);

    EXPECT_EQ(stack.in_systematic_phase(), false);
    EXPECT_EQ(stack.systematic_count(), 4U);

    // Initialize the stack again - we leave the m_is_symbol_uncoded
    // which will indicate that all symbols are available
    stack.initialize(factory);

    EXPECT_EQ(stack.systematic_count(), 0U);
    EXPECT_EQ(stack.symbols(), factory.symbols());
    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 0U);

}


