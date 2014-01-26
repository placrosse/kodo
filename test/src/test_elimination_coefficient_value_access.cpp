// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>
#include <fifi/field_types.hpp>
#include <kodo/elimination_coefficient_value_access.hpp>
#include <kodo/elimination_coefficient_offset.hpp>
#include <kodo/storage_block_info.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        /// Helper class to test coefficient value access layer
        class dummy_layer
        {
        public:

            typedef uint8_t value_type;

        public:

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t elimination_offset() const
            {
                return m_elimination_offset;
            }

            value_type coefficient_value(const value_type* coefficients,
                                         uint32_t index) const
            {
                m_coefficients = coefficients;
                m_index = index;
                return m_value;
            }

            void set_coefficient_value(value_type* coefficients, uint32_t index,
                                       value_type value) const
            {
                m_coefficients = coefficients;
                m_index = index;
                m_value = value;
            }

        public:

            uint32_t m_symbols;
            uint32_t m_elimination_offset;

            mutable const value_type* m_coefficients;
            mutable uint32_t m_index;
            mutable value_type m_value;

        };

        /// Helper stack for testing the coefficient access layer
        class dummy_stack :
            public elimination_coefficient_value_access<dummy_layer>
        { };
    }
}

TEST(TestEliminationCoefficientValueAccess, api)
{

    typedef kodo::dummy_stack stack_type;
    typedef stack_type::value_type value_type;

    stack_type stack;
    stack.m_symbols = 10;
    stack.m_elimination_offset = 2;

    // Test layer::coefficient_value(const value_type*, uint32_t) const
    stack.m_value = 5;
    std::vector<uint8_t> buffer(sizeof(value_type));
    uint32_t index = 4;

    value_type c = stack.coefficient_value(&buffer[0], index);
    EXPECT_EQ(stack.m_value, c);
    EXPECT_EQ(stack.m_coefficients, &buffer[0]);
    EXPECT_EQ(stack.m_index, 6U);

    stack.set_coefficient_value(&buffer[0], index, c);
    EXPECT_EQ(stack.m_value, c);
    EXPECT_EQ(stack.m_coefficients, &buffer[0]);
    EXPECT_EQ(stack.m_index, 6U);

}

