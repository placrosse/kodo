// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_trace.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy
        {
            void trace(std::ostream&);
        };

        struct dummy_parent : public dummy
        { };

        struct dummy_false
        { };

    }
}

TEST(TestHasTrace, detect)
{
    EXPECT_FALSE(kodo::has_trace<uint32_t>::value);
    EXPECT_FALSE(kodo::has_trace<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_trace<kodo::dummy>::value);
    EXPECT_TRUE(kodo::has_trace<kodo::dummy_parent>::value);

    typedef kodo::sliding_window_decoder<
        fifi::binary8> decoder_trace_off;

    typedef kodo::sliding_window_decoder<
        fifi::binary8,kodo::enable_trace> decoder_trace_on;

    EXPECT_FALSE(kodo::has_trace<decoder_trace_off>::value);
    EXPECT_TRUE(kodo::has_trace<decoder_trace_on>::value);
}
