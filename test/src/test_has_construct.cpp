// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_construct.cpp Unit tests for the
///       has_construct class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_construct.hpp>
#include <kodo/rlnc/sliding_window_encoder.hpp>

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
            void construct(Factory& the_factory);
        };

        struct dummy_two
        {
            template<class Factory>
            void construct(Factory& the_factory)
            {
                the_factory.ok();
            }

        };

        struct dummy_one_parent : public dummy_one
        { };

        struct dummy_two_parent : public dummy_two
        { };

        struct dummy_false
        { };
    }
}

TEST(TestHasConstruct, detect)
{
    EXPECT_FALSE(kodo::has_construct<uint32_t>::value);
    EXPECT_FALSE(kodo::has_construct<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_construct<kodo::dummy_one>::value);
    EXPECT_TRUE(kodo::has_construct<kodo::dummy_one_parent>::value);
    EXPECT_TRUE(kodo::has_construct<kodo::dummy_two>::value);
    EXPECT_TRUE(kodo::has_construct<kodo::dummy_two_parent>::value);

    typedef kodo::sliding_window_encoder<fifi::binary8> encoder_type;
    EXPECT_TRUE(kodo::has_construct<encoder_type>::value);
}
