// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_forward_coefficient_iterator.cpp Unit test
///       for the forward_coefficient_iterator

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/forward_coefficient_iterator.hpp>
#include <fifi/fifi_utils.hpp>

namespace kodo
{

    template<class FieldType>
    class dummy_layer
    {
    public:

        // The field type used in our test
        typedef FieldType field_type;

    public:

        uint32_t symbols() const
        {
            return m_symbols;
        }

    public:

        uint32_t m_symbols;

    };


    template<class FieldType>
    class dummy_stack :
        public forward_coefficient_iterator<
               dummy_layer<FieldType> >
    { };

}

/// Run the tests typical coefficients stack
TEST(TestForwardCoefficientIterator, binary)
{
    typedef fifi::binary field_type;

    kodo::dummy_stack<field_type> stack;
    stack.m_symbols = 4U;

    auto it = stack.coefficient_iterator();

    EXPECT_FALSE(it.at_end());
    EXPECT_EQ(it.index(), 0U);

    it.advance();

    EXPECT_FALSE(it.at_end());
    EXPECT_EQ(it.index(), 1U);

    it.advance();

    EXPECT_FALSE(it.at_end());
    EXPECT_EQ(it.index(), 2U);

    it.advance();

    EXPECT_FALSE(it.at_end());
    EXPECT_EQ(it.index(), 3U);

    it.advance();

    EXPECT_TRUE(it.at_end());

    it = stack.coefficient_iterator(2);

    EXPECT_FALSE(it.at_end());
    EXPECT_EQ(it.index(), 2U);

    it.advance();

    EXPECT_FALSE(it.at_end());
    EXPECT_EQ(it.index(), 3U);

    it.advance();

    EXPECT_TRUE(it.at_end());
}



