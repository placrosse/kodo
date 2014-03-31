// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <sstream>

#include <gtest/gtest.h>

#include <kodo/debug.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_one
        {
            void debug(std::ostream& out)
            {
                out << "in debug" << std::endl;
            }

        };

        struct dummy_two
        { };

    }
}

TEST(TestDebug, invoke)
{

    {
        // Check with a standard object
        kodo::dummy_one d;
        std::stringstream ss;

        kodo::debug(d, ss);
        EXPECT_EQ(ss.str(), std::string("in debug\n"));
    }

    {
        // Check that the code compiles even with an object that
        // does not have the debug function
        kodo::dummy_two d;

        if(kodo::has_debug<kodo::dummy_two>::value)
        {
            kodo::debug(d, std::cout);
        }
    }

    /// @todo enable with real stack
    // {
    //     // Check that the code compiles with a normal stack
    //     typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

    //     encoder_type::factory factory(10,10);
    //     auto encoder = factory.build();

    //     EXPECT_TRUE(encoder->is_systematic_on());

    //     kodo::set_systematic_on(encoder);
    //     EXPECT_TRUE(encoder->is_systematic_on());

    //     kodo::set_systematic_off(encoder);
    //     EXPECT_FALSE(encoder->is_systematic_on());

    //     kodo::set_systematic_on(encoder);
    //     EXPECT_TRUE(encoder->is_systematic_on());


    // }

}



