// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_offset_coefficient_value_access.cpp Unit tests for the
///       offset_coefficient_value_access class

#include <cstdint>

#include <gtest/gtest.h>
#include <fifi/binary.hpp>
#include <fifi/binary8.hpp>
#include <kodo/offset_coefficient_value_access.hpp>
#include <kodo/storage_block_size.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        /// Helper class to test coefficient value access layer
        template<class FieldType>
        class test_layer
        {
        public:

            /// Field used
            typedef FieldType field_type;

            /// The value type
            typedef typename field_type::value_type value_type;

        public:

            test_layer()
                : m_coeffcients_elements(0)
            { }

            uint32_t coefficients_elements() const
            {
                return m_coeffcients_elements;
            }

        public:

            uint32_t m_coeffcients_elements;

        };

        /// Helper stack for testing the coefficient access layer
        template<class FieldType>
        class test_stack :
            public offset_coefficient_value_access<test_layer<FieldType> >
        { };
    }
}

TEST(TestOffsetCoefficientValueAccess, api)
{
    {

    typedef fifi::binary field_type;
    typedef field_type::value_type value_type;
    typedef kodo::test_stack<field_type> stack_type;

    stack_type stack;
    stack.m_coeffcients_elements = 6;

    std::vector<value_type> coefficients(
        fifi::elements_to_length<field_type>(stack.m_coeffcients_elements));

    EXPECT_EQ(stack.coefficient_offset(), 0U);

    stack.set_coefficient_value(&coefficients[0], 0, 1U);
    stack.set_coefficient_value(&coefficients[0], 1, 0U);
    stack.set_coefficient_value(&coefficients[0], 2, 1U);
    stack.set_coefficient_value(&coefficients[0], 3, 0U);
    stack.set_coefficient_value(&coefficients[0], 4, 1U);
    stack.set_coefficient_value(&coefficients[0], 5, 0U);

    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 0), 1U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 1), 0U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 2), 1U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 3), 0U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 4), 1U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 5), 0U);

    stack.set_coefficient_offset(3);
    EXPECT_EQ(stack.coefficient_offset(), 3U);

    stack.set_coefficient_value(&coefficients[0], 0, 1U);
    stack.set_coefficient_value(&coefficients[0], 1, 0U);
    stack.set_coefficient_value(&coefficients[0], 2, 1U);

    stack.set_coefficient_offset(0);
    EXPECT_EQ(stack.coefficient_offset(), 0U);

    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 3), 1U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 4), 0U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 5), 1U);

    }

    {

    typedef fifi::binary8 field_type;
    typedef field_type::value_type value_type;
    typedef kodo::test_stack<field_type> stack_type;

    stack_type stack;
    stack.m_coeffcients_elements = 6;

    std::vector<value_type> coefficients(
        fifi::elements_to_length<field_type>(stack.m_coeffcients_elements));

    EXPECT_EQ(stack.coefficient_offset(), 0U);

    stack.set_coefficient_value(&coefficients[0], 0, 111U);
    stack.set_coefficient_value(&coefficients[0], 1, 222U);
    stack.set_coefficient_value(&coefficients[0], 2, 0U);
    stack.set_coefficient_value(&coefficients[0], 3, 111U);
    stack.set_coefficient_value(&coefficients[0], 4, 99U);
    stack.set_coefficient_value(&coefficients[0], 5, 1U);

    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 0), 111U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 1), 222U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 2), 0U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 3), 111U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 4), 99U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 5), 1U);

    stack.set_coefficient_offset(3);
    EXPECT_EQ(stack.coefficient_offset(), 3U);

    stack.set_coefficient_value(&coefficients[0], 0, 60U);
    stack.set_coefficient_value(&coefficients[0], 1, 10U);
    stack.set_coefficient_value(&coefficients[0], 2, 99U);

    stack.set_coefficient_offset(0);
    EXPECT_EQ(stack.coefficient_offset(), 0U);

    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 3), 60U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 4), 10U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 5), 99U);

    }

}
