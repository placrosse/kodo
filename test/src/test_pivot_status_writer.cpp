// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_pivot_status_writer.cpp Unit tests for the
///       kodo::pivot_status_writer layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/pivot_status_writer.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // pivot_status_writer layer.
        struct dummy_layer
        {

            void set_symbol_missing(uint32_t index)
            {
                m_set_symbol_missing = index;
            }

            void set_symbol_seen(uint32_t index)
            {
                m_set_symbol_seen = index;
            }

            void set_symbol_decoded(uint32_t index)
            {

            }

            uint32_t m_set_symbol_missing;
            uint32_t m_set_symbol_seen;
            uint32_t m_set_symbol_decoded;

        };

        // Instantiate a stack containing the pivot_status_writer
        class dummy_stack
            : public pivot_status_writer<
                     dummy_layer>
          { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPivotStatusWriter, api)
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


