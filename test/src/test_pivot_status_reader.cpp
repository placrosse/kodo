// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_pivot_status_reader.cpp Unit tests for the
///       kodo::pivot_status_reader layer

#include <cstdint>
#include <gtest/gtest.h>
#include <fifi/fifi_utils.hpp>

#include <kodo/pivot_status_reader.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // pivot_status_reader layer.
        struct dummy_layer
        {
        public:

            typedef uint32_t rank_type;

        public:

            struct factory_base
            {

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t max_symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t symbols() const
                {
                    return m_symbols;
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;
                uint32_t m_symbols;

            };

        public:

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

        // Instantiate a stack containing the pivot_status_reader
        class dummy_stack
            : public pivot_status_reader<
                     dummy_layer>
        {
        public:
            using factory = basic_factory<dummy_stack>;
        };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPivotStatusReader, api)
{

    kodo::dummy_stack stack;
    kodo::dummy_stack::factory factory(10, 100);

    factory.m_symbols = 9;

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.pivot_status_size(), 2U);

    EXPECT_EQ(stack.remote_rank(), 0U);

    EXPECT_EQ(stack.remote_is_symbol_pivot(0), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(1), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(2), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(3), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(4), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(5), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(6), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(7), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(8), 0U);

    std::vector<uint8_t> buffer(stack.pivot_status_size(), 0);

    // We use fifi to set and get the bits
    fifi::set_value<fifi::binary>(&buffer[0], 0, 0U);
    fifi::set_value<fifi::binary>(&buffer[0], 1, 1U);
    fifi::set_value<fifi::binary>(&buffer[0], 2, 0U);
    fifi::set_value<fifi::binary>(&buffer[0], 3, 0U);
    fifi::set_value<fifi::binary>(&buffer[0], 4, 0U);
    fifi::set_value<fifi::binary>(&buffer[0], 5, 1U);
    fifi::set_value<fifi::binary>(&buffer[0], 6, 0U);
    fifi::set_value<fifi::binary>(&buffer[0], 7, 1U);
    fifi::set_value<fifi::binary>(&buffer[0], 8, 1U);

    stack.read_pivot_status(&buffer[0]);

    EXPECT_EQ(stack.remote_rank(), 4U);

    EXPECT_EQ(stack.remote_is_symbol_pivot(0), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(1), 1U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(2), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(3), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(4), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(5), 1U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(6), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(7), 1U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(8), 1U);

    stack.initialize(factory);

    EXPECT_EQ(stack.remote_rank(), 0U);

    EXPECT_EQ(stack.remote_is_symbol_pivot(0), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(1), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(2), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(3), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(4), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(5), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(6), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(7), 0U);
    EXPECT_EQ(stack.remote_is_symbol_pivot(8), 0U);

}
