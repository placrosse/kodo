// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <sstream>

#include <gtest/gtest.h>

#include <kodo/trace.hpp>
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
            template<class Filter>
            void trace(std::ostream& out, const Filter& filter)
            {
                (void) filter;
                out << "in trace" << std::endl;
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
    if (kodo::has_trace<Decoder>::value)
    {
        kodo::trace(decoder, ss);
    }

    return ss.str();
}

TEST(TestTrace, invoke)
{

    {
        // Check with a standard object
        kodo::dummy_one d;
        std::stringstream ss;

        kodo::trace(d, ss);
        EXPECT_EQ(ss.str(), std::string("in trace\n"));
    }

    {
        // Check that the code compiles even with an object that
        // does not have the trace function
        kodo::dummy_two d;

        if(kodo::has_trace<kodo::dummy_two>::value)
        {
            kodo::trace(d, std::cout);
        }
    }

    // Define two real stacks with trace on or off
    typedef kodo::sliding_window_decoder<
        fifi::binary8> decoder_trace_off;

    typedef kodo::sliding_window_decoder<
        fifi::binary8,kodo::enable_trace> decoder_trace_on;

    EXPECT_TRUE(test_output<decoder_trace_off>() == "");

    // We expect the trace on to produce some trace output
    EXPECT_FALSE(test_output<decoder_trace_on>() == "");
}

/// Check that the trace functionality compiles with different stacks
template<class Stack>
void check_stack()
{
    if (kodo::has_trace<Stack>::value)
    {
        std::stringstream ss;

        typename Stack::factory factory(10,10);
        auto s = factory.build();

        kodo::trace(s, ss);
    }
}

namespace
{
    using rlnc_decoder8 =
        kodo::full_rlnc_decoder<fifi::binary8, kodo::enable_trace>;

    using rlnc_encoder8 =
        kodo::full_rlnc_encoder<fifi::binary8, kodo::enable_trace>;

    using shallow_rlnc_decoder8 =
        kodo::shallow_full_rlnc_decoder<fifi::binary8, kodo::enable_trace>;

    using shallow_rlnc_encoder8 =
        kodo::shallow_full_rlnc_encoder<fifi::binary8, kodo::enable_trace>;
}

TEST(TestTrace, stacks)
{
    check_stack<rlnc_decoder8>();
    check_stack<rlnc_encoder8>();
    check_stack<shallow_rlnc_decoder8>();
    check_stack<shallow_rlnc_encoder8>();
}
