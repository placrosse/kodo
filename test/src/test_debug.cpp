// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <sstream>

#include <gtest/gtest.h>

#include <kodo/debug.hpp>
#include <kodo/rlnc/sliding_window_encoder.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>

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

template<class Decoder>
inline std::string test_output()
{
    typename Decoder::factory factory(10, 100);
    auto decoder = factory.build();

    std::stringstream ss;
    if (kodo::has_debug<Decoder>::value)
    {
        kodo::debug(decoder, ss);
    }

    return ss.str();
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

    // Define two real stacks with debug on or off
    typedef kodo::sliding_window_decoder<
        fifi::binary8> decoder_debug_off;

    typedef kodo::sliding_window_decoder<
        fifi::binary8,kodo::enable_debug> decoder_debug_on;

    EXPECT_TRUE(test_output<decoder_debug_off>() == "");

    // We expect the debug on to produce some debug output
    EXPECT_FALSE(test_output<decoder_debug_on>() == "");

}
