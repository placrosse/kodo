// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/reed_solomon/reed_solomon_codes.hpp>

#include "kodo_unit_test/helper_test_basic_api.hpp"

namespace
{
    template<class Field>
    using encoder = kodo::reed_solomon_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using decoder = kodo::reed_solomon_decoder<Field, kodo::disable_trace>;

    using encoder8 = encoder<fifi::binary8>;

    using decoder8 = decoder<fifi::binary8>;
}

TEST(TestReedSolomonCodes, test_basic_api)
{
    /// @todo Maybe we can find a way to better support
    ///       MDS/rate-limited codes in our unit tests helpers. Since
    ///       we do not check whether MDS codes actually do MDS
    ///       decoding (i.e. decode with k out of n symbols) and also
    ///       there is a possibility we can run out of redundancy in
    ///       tests with erasures.

    run_test_basic_api<encoder8,decoder8>(255, 1600);
}
