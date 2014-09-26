// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_default_systematic_phase.cpp Unit tests for the
///       kodo::default_systematic_phase layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/default_systematic_phase.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // default_systematic_phase layer.
        struct dummy_layer
        {
        public:

            struct factory_base
            { };

        public:

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }

            bool in_systematic_phase() const
            {
                return m_in_systematic_phase;
            }

            bool m_in_systematic_phase;
        };

        // Instantiate a stack containing the default_systematic_phase
        template<bool SystematicOn>
        class dummy_stack
            : public default_systematic_phase<SystematicOn,
                     dummy_layer>
          { };
    }
}

template<bool SystematicOn, template <bool> class Stack>
inline void test()
{
    typedef Stack<SystematicOn> stack_type;

    stack_type stack;

    stack.m_in_systematic_phase = false;

    EXPECT_EQ(stack.is_systematic_on(), SystematicOn);
    EXPECT_EQ(stack.in_systematic_phase(), false);

    stack.set_systematic_off();

    EXPECT_EQ(stack.in_systematic_phase(), false);
    EXPECT_EQ(stack.is_systematic_on(), false);

    stack.set_systematic_on();

    EXPECT_EQ(stack.in_systematic_phase(), false);
    EXPECT_EQ(stack.is_systematic_on(), true);

    stack.m_in_systematic_phase = true;

    EXPECT_EQ(stack.in_systematic_phase(), true);
    EXPECT_EQ(stack.is_systematic_on(), true);

    stack.set_systematic_off();

    EXPECT_EQ(stack.in_systematic_phase(), false);
    EXPECT_EQ(stack.is_systematic_on(), false);

}

/// Run the tests typical coefficients stack
TEST(TestDefaultSystematicPhase, api_false)
{
    test<false, kodo::dummy_stack>();
}

/// Run the tests typical coefficients stack
TEST(TestDefaultSystematicPhase, api_true)
{
    test<true, kodo::dummy_stack>();
}
