// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_coefficient_value_access.cpp Unit tests for the
///       coefficient_value_access class

#include <cstdint>

#include <gtest/gtest.h>
#include <fifi/field_types.hpp>
#include <kodo/coefficient_value_access.hpp>
#include <kodo/storage_block_info.hpp>

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

        };

        /// Helper stack for testing the coefficient access layer
        template<class FieldType>
        class test_stack :
            public coefficient_value_access<test_layer<FieldType> >
        { };
    }
}

TEST(TestCoefficientValueAccess, api)
{
    {

    typedef fifi::binary field_type;
    typedef field_type::value_type value_type;
    typedef kodo::test_stack<field_type> stack_type;

    stack_type stack;
    uint32_t symbols = 4;

    std::vector<value_type> coefficients(
        fifi::elements_to_length<field_type>(symbols));

    stack.set_coefficient_value(&coefficients[0], 0, 1U);
    stack.set_coefficient_value(&coefficients[0], 1, 0U);
    stack.set_coefficient_value(&coefficients[0], 2, 1U);
    stack.set_coefficient_value(&coefficients[0], 3, 0U);

    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 0), 1U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 1), 0U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 2), 1U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 3), 0U);

    }

    {

    typedef fifi::binary8 field_type;
    typedef field_type::value_type value_type;
    typedef kodo::test_stack<field_type> stack_type;

    stack_type stack;
    uint32_t symbols = 4;

    std::vector<value_type> coefficients(
        fifi::elements_to_length<field_type>(symbols));

    stack.set_coefficient_value(&coefficients[0], 0, 111U);
    stack.set_coefficient_value(&coefficients[0], 1, 222U);
    stack.set_coefficient_value(&coefficients[0], 2, 0U);
    stack.set_coefficient_value(&coefficients[0], 3, 111U);

    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 0), 111U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 1), 222U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 2), 0U);
    EXPECT_EQ(stack.coefficient_value(&coefficients[0], 3), 111U);

    }

}


