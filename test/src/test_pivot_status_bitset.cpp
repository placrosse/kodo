// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_pivot_status_bitset.cpp Unit tests for the
///       kodo::pivot_status_bitset layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/pivot_status_bitset.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // pivot_status_bitset layer.
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
                (void) the_factory;
            }
        };

        struct dummy_factory
        {

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

        // Instantiate a stack containing the pivot_status_bitset
        class dummy_stack
            : public pivot_status_bitset<
                     dummy_layer>
          { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPivotStatusBitset, api)
{

    kodo::dummy_stack stack;
    kodo::dummy_factory factory;

    factory.m_max_symbols = 10;
    factory.m_symbols = 5;

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.pivot_status_size(), 1U);

    factory.m_symbols = 7;
    stack.initialize(factory);

    EXPECT_EQ(stack.pivot_status_size(), 1U);

    factory.m_symbols = 9;
    stack.initialize(factory);

    EXPECT_EQ(stack.pivot_status_size(), 2U);

}


