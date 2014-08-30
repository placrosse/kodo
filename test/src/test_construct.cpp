// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/construct.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_one
        {
            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
                m_construct();
            }

            stub::call<void()> m_construct;
        };

        struct dummy_two
        { };

        struct dummy_factory
        { };
    }
}

TEST(TestConstruct, invoke)
{
    {
        // Check with a standard object
        kodo::dummy_factory f;
        kodo::dummy_one d;
        kodo::construct(d, f);
        EXPECT_TRUE((bool) d.m_construct.expect_calls().with());
    }

    {
        // Check that the code compiles even with an object that
        // does not have the construct function
        kodo::dummy_factory f;
        kodo::dummy_two d;

        if(kodo::has_construct<kodo::dummy_two>::value)
        {
            kodo::construct(d, f);
        }
    }

    {
        // Check that the code compiles with a normal stack
        using encoder_type = kodo::full_rlnc_encoder<fifi::binary8>;

        encoder_type::factory factory(10,10);

        encoder_type e;
        kodo::construct(e, factory);
    }
}
