// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/initialize.hpp>
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
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
                m_initialize();
            }

            stub::call<void()> m_initialize;
        };

        struct dummy_two
        { };

        struct dummy_factory
        { };
    }
}

TEST(TestInitialize, invoke)
{
    {
        // Check with a standard object
        kodo::dummy_factory f;
        kodo::dummy_one d;
        kodo::initialize(d, f);
        EXPECT_TRUE(d.m_initialize.called_once_with());
    }

    {
        // Check that the code compiles even with an object that
        // does not have the initialize function
        kodo::dummy_factory f;
        kodo::dummy_two d;

        if(kodo::has_initialize<kodo::dummy_two>::value)
        {
            kodo::initialize(d, f);
        }
    }

    {
        // Check that the code compiles with a normal stack
        using encoder_type = kodo::full_rlnc_encoder<fifi::binary8>;

        encoder_type::factory factory(10,10);

        auto encoder = factory.build();

        kodo::initialize(*encoder, factory);
    }
}
