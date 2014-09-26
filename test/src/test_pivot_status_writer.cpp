// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_pivot_status_writer.cpp Unit tests for the
///       kodo::pivot_status_writer layer

#include <cstdint>
#include <gtest/gtest.h>
#include <fifi/fifi_utils.hpp>

#include <kodo/pivot_status_writer.hpp>
#include <kodo/basic_factory.hpp>

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

            void set_symbol_missing(uint32_t index)
            {
                m_set_symbol_missing = index;
            }

            void set_symbol_seen(uint32_t index)
            {
                m_set_symbol_seen = index;
            }

            void set_symbol_uncoded(uint32_t index)
            {
                m_set_symbol_uncoded = index;
            }

            uint32_t m_set_symbol_missing;
            uint32_t m_set_symbol_seen;
            uint32_t m_set_symbol_uncoded;

        };



        // Instantiate a stack containing the pivot_status_writer
        class dummy_stack
            : public pivot_status_writer<
                     dummy_layer>
        {
        public:
            using factory = basic_factory<dummy_stack>;
        };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPivotStatusWriter, api)
{

    kodo::dummy_stack stack;
    kodo::dummy_stack::factory factory(10, 100);

    factory.m_symbols = 9;

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.pivot_status_size(), 2U);

    stack.set_symbol_uncoded(1U);
    EXPECT_EQ(stack.m_set_symbol_uncoded, 1U);

    stack.set_symbol_seen(5U);
    EXPECT_EQ(stack.m_set_symbol_seen, 5U);

    stack.set_symbol_missing(6U);
    EXPECT_EQ(stack.m_set_symbol_missing, 6U);

    stack.set_symbol_uncoded(7U);
    EXPECT_EQ(stack.m_set_symbol_uncoded, 7U);

    stack.set_symbol_uncoded(8U);
    EXPECT_EQ(stack.m_set_symbol_uncoded, 8U);


    std::vector<uint8_t> buffer(stack.pivot_status_size(), 0);

    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 0), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 1), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 2), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 3), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 4), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 5), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 6), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 7), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 8), 0U);

    stack.write_pivot_status(&buffer[0]);

    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 0), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 1), 1U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 2), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 3), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 4), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 5), 1U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 6), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 7), 1U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 8), 1U);

    stack.initialize(factory);
    stack.write_pivot_status(&buffer[0]);

    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 0), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 1), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 2), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 3), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 4), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 5), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 6), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 7), 0U);
    EXPECT_EQ(fifi::get_value<fifi::binary>(&buffer[0], 8), 0U);

}
