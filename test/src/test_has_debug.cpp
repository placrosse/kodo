// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_debug.hpp>
#include <kodo/rlnc/full_vector_codes.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy
        {
            void debug(std::ostream&);
        };

        struct dummy_parent : public dummy
        { };

        struct dummy_false
        { };

    }
}

TEST(TestHasDebug, detect)
{
    EXPECT_FALSE(kodo::has_debug<uint32_t>::value);
    EXPECT_FALSE(kodo::has_debug<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_debug<kodo::dummy>::value);
    EXPECT_TRUE(kodo::has_debug<kodo::dummy_parent>::value);

    /// @todo replace with sliding_window_decoder
    // typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;
    // EXPECT_TRUE(kodo::has_debug<encoder_type::factory>::value);
}



