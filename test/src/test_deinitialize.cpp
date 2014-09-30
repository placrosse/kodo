// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/deinitialize.hpp>
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
            void deinitialize(Factory& the_factory)
            {
                (void) the_factory;
                m_deinitialize();
            }

            stub::call<void()> m_deinitialize;
        };

        struct dummy_two
        { };

        struct dummy_factory
        { };
    }
}

TEST(TestDeinitialize, invoke)
{
    {
        // Check with a standard object
        kodo::dummy_factory f;
        kodo::dummy_one d;
        kodo::deinitialize(d, f);
        EXPECT_TRUE((bool) d.m_deinitialize.expect_calls().with());
    }

    {
        // Check that the code compiles even with an object that
        // does not have the deinitialize function
        kodo::dummy_factory f;
        kodo::dummy_two d;

        if(kodo::has_deinitialize<kodo::dummy_two>::value)
        {
            kodo::deinitialize(d, f);
        }
    }

    {
        // Check that the code compiles with a normal stack
        using encoder_type = kodo::full_rlnc_encoder<fifi::binary8>;

        encoder_type::factory factory(10,10);

        auto encoder = factory.build();

        kodo::deinitialize(*encoder, factory);
    }
}
