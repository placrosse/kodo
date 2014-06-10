// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <stub/call.hpp>

#include <kodo/nested_toggle_systematic.hpp>
#include "kodo_unit_test/helper_test_nested_stack.hpp"

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // This will represent the nested stack returned by the actual
        // stack
        struct nested_stack
        {
            void set_systematic_on()
            {
                return m_set_systematic_on();
            }

            void set_systematic_off()
            {
                return m_set_systematic_off();
            }

            stub::call<void ()> m_set_systematic_on;
            stub::call<void ()> m_set_systematic_off;
        };

        // The layer providing the set_systeamtic_on
        // function
        class dummy_layer
        {
        public:

            nested_stack* nested()
            {
                return &m_nested;
            }

            const nested_stack* nested() const
            {
                return &m_nested;
            }

             nested_stack m_nested;
        };

        // The layer providing the set_systeamtic_on
        // function
        class systematic_on : public dummy_layer
        {
        public:

            void set_systematic_on()
            {
                return m_set_systematic_on();
            }

            stub::call<void ()> m_set_systematic_on;
        };

        // The layer providing the set_systeamtic_on
        // function
        class systematic_off : public dummy_layer
        {
        public:

            void set_systematic_off()
            {
                return m_set_systematic_off();
            }

            stub::call<void ()> m_set_systematic_off;
        };

        // The layer providing the set_systeamtic_on
        // function
        class systematic_all : public dummy_layer
        {
        public:

            void set_systematic_off()
            {
                return m_set_systematic_off();
            }

            void set_systematic_on()
            {
                return m_set_systematic_on();
            }

            stub::call<void ()> m_set_systematic_off;
            stub::call<void ()> m_set_systematic_on;
        };

        // Helper stack
        class dummy_stack_none : public nested_toggle_systematic<dummy_layer>
        { };

        // Helper stack
        class dummy_stack_on : public nested_toggle_systematic<systematic_on>
        { };

        // Helper stack
        class dummy_stack_off : public nested_toggle_systematic<systematic_off>
        { };

        // Helper stack
        class dummy_stack_all : public nested_toggle_systematic<systematic_all>
        { };
    }
}

TEST(TestNestedToggleSystematic, api)
{
    {
        kodo::dummy_stack_none stack;
        stack.set_systematic_on();
        stack.set_systematic_off();

        EXPECT_TRUE(stack.m_nested.m_set_systematic_on.called_once_with());
        EXPECT_TRUE(stack.m_nested.m_set_systematic_off.called_once_with());
    }

    {
        kodo::dummy_stack_on stack;
        stack.set_systematic_on();
        stack.set_systematic_off();

        EXPECT_TRUE(stack.m_nested.m_set_systematic_on.called_once_with());
        EXPECT_TRUE(stack.m_nested.m_set_systematic_off.called_once_with());
        EXPECT_TRUE(stack.m_set_systematic_on.called_once_with());
    }

    {
        kodo::dummy_stack_off stack;
        stack.set_systematic_on();
        stack.set_systematic_off();

        EXPECT_TRUE(stack.m_nested.m_set_systematic_on.called_once_with());
        EXPECT_TRUE(stack.m_nested.m_set_systematic_off.called_once_with());
        EXPECT_TRUE(stack.m_set_systematic_off.called_once_with());
    }

    {
        kodo::dummy_stack_all stack;
        stack.set_systematic_on();
        stack.set_systematic_off();

        EXPECT_TRUE(stack.m_nested.m_set_systematic_on.called_once_with());
        EXPECT_TRUE(stack.m_nested.m_set_systematic_off.called_once_with());
        EXPECT_TRUE(stack.m_set_systematic_on.called_once_with());
        EXPECT_TRUE(stack.m_set_systematic_off.called_once_with());
    }
}
