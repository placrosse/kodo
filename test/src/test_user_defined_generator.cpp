// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_user_defined_generator.hpp Unit tests for the uniform coefficient
///       generators

#include <gtest/gtest.h>

#include <kodo/user_defined_generator.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        class dummy_layer
        {
        public:

            template<class Factory>
            void initialize(Factory &the_factory)
            {
                (void) the_factory;
            }

            uint32_t rank() const
            {
                // Just to satisfy the assert
                return m_symbols;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t coefficient_vector_size() const
            {
                return m_coefficient_vector_size;
            }

        public:

            uint32_t m_symbols;
            uint32_t m_coefficient_vector_size;

        };

        class dummy_stack
            : public user_defined_generator<dummy_layer>
        { };

        class dummy_factory
        { };

    }
}

TEST(TestUserDefinedGenerator, api)
{
    kodo::dummy_factory factory;
    kodo::dummy_stack stack;

    stack.m_symbols = 10U;
    stack.m_coefficient_vector_size = 10U;

    // We generate some extra data in the vectors to make sure that
    // this data is not touched
    uint32_t extra = 10;

    std::vector<uint8_t> coefficients_in(
        stack.m_coefficient_vector_size + extra, 'x');

    std::vector<uint8_t> coefficients_out(
        stack.m_coefficient_vector_size + extra, 'y');

    stack.initialize(factory);
    EXPECT_EQ(stack.user_coefficients(), nullptr);

    stack.set_user_coefficients(&coefficients_in[0]);
    EXPECT_EQ(stack.user_coefficients(), &coefficients_in[0]);

    stack.generate(&coefficients_out[0]);
    EXPECT_EQ(stack.user_coefficients(), &coefficients_in[0]);

    stack.initialize(factory);
    EXPECT_EQ(stack.user_coefficients(), nullptr);

    for(const auto& c : coefficients_in)
    {
        EXPECT_EQ(c, 'x');
    }

    for(uint32_t i = 0; i < coefficients_out.size(); ++i)
    {
        if(i < stack.m_coefficient_vector_size)
        {
            // Should now be equal to coefficients_in
            EXPECT_EQ(coefficients_out[i], 'x');
        }
        else
        {
            // Untouched
            EXPECT_EQ(coefficients_out[i], 'y');
        }
    }

}

