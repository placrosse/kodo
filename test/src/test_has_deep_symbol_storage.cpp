// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_shallow_symbol_storage.cpp Unit tests for the
///       has_shallow_symbol_storage class

#include <gtest/gtest.h>

#include <kodo/has_deep_symbol_storage.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        struct dummy
        { };

        using deep_decoder = kodo::full_rlnc_decoder<fifi::binary>;
        using deep_encoder = kodo::full_rlnc_encoder<fifi::binary>;
        using shallow_decoder = kodo::shallow_full_rlnc_decoder<fifi::binary>;
        using shallow_encoder = kodo::shallow_full_rlnc_encoder<fifi::binary>;
    }
}

/// Tests the has_deep_symbol_storage template
TEST(TestHasDeepSymbolStorage, detect)
{
    EXPECT_FALSE(kodo::has_deep_symbol_storage<kodo::dummy>::value);
    EXPECT_TRUE(kodo::has_deep_symbol_storage<kodo::deep_decoder>::value);
    EXPECT_TRUE(kodo::has_deep_symbol_storage<kodo::deep_encoder>::value);
    EXPECT_FALSE(kodo::has_deep_symbol_storage<kodo::shallow_decoder>::value);
    EXPECT_FALSE(kodo::has_deep_symbol_storage<kodo::shallow_encoder>::value);
}
