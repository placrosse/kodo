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
                m_is_symbol_initialized.resize(m_symbols, false);
            }

            void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
            {
                (void) symbol_data;
                (void) symbol_index;
            }

            uint32_t rank() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_is_symbol_initialized[i];
                }
                return rank;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            bool is_symbol_initialized(uint32_t index) const
            {
                (void) index;
                return m_is_symbol_initialized[index];
            }

            uint32_t m_symbols;
            std::vector<bool> m_is_symbol_initialized;
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

    stack.m_is_symbol_initialized[0] = true;

    EXPECT_EQ(stack.symbols(), factory.symbols());
    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 0);

    uint8_t* symbol = 0;

    EXPECT_EQ(stack.next_systematic_symbol(), 0);
    stack.encode_symbol(symbol, 0);
    EXPECT_EQ(stack.in_systematic_phase(), false);

    stack.m_is_symbol_initialized[2] = true;
    stack.m_is_symbol_initialized[3] = true;

    EXPECT_EQ(stack.rank(), 3);
    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 2);

    // We resend the first symbol nothing should change
    stack.encode_symbol(symbol, 0);

    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 2);
    stack.encode_symbol(symbol, 2);

    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 3);
    stack.encode_symbol(symbol, 3);
    EXPECT_EQ(stack.in_systematic_phase(), false);

    stack.m_is_symbol_initialized[1] = true;

    EXPECT_EQ(stack.rank(), 4);
    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.next_systematic_symbol(), 1);

    stack.encode_symbol(symbol, 1);
    EXPECT_EQ(stack.in_systematic_phase(), false);

}


