// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_deinitialize.cpp Unit tests for the
///       has_deinitialize class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_deinitialize.hpp>
#include <kodo/object/file_decoder.hpp>
#include <kodo/rlnc/shallow_full_rlnc_decoder.hpp>

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
            void deinitialize(Factory& the_factory);
        };

        struct dummy_two
        {
            template<class Factory>
            void deinitialize(Factory& the_factory)
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

TEST(TestHasDeinitialize, detect)
{
    EXPECT_FALSE(kodo::has_deinitialize<uint32_t>::value);
    EXPECT_FALSE(kodo::has_deinitialize<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_deinitialize<kodo::dummy_one>::value);
    EXPECT_TRUE(kodo::has_deinitialize<kodo::dummy_one_parent>::value);
    EXPECT_TRUE(kodo::has_deinitialize<kodo::dummy_two>::value);
    EXPECT_TRUE(kodo::has_deinitialize<kodo::dummy_two_parent>::value);

    using codec = kodo::object::file_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary8>>;

    EXPECT_TRUE(kodo::has_deinitialize<codec>::value);
}
