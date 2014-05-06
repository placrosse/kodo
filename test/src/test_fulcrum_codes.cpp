// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_fulcrum_codes.cpp Unit tests for the Fulcrum coding scheme

#include <ctime>

#include <gtest/gtest.h>

#include <kodo/rlnc/fulcrum_codes.hpp>

#include "kodo_unit_test/helper_test_basic_api.hpp"
#include "kodo_unit_test/helper_test_reuse_api.hpp"
#include "kodo_unit_test/helper_test_initialize_api.hpp"


namespace
{
    template<class Field>
    using encoder = kodo::fulcrum_encoder<Field, kodo::disable_trace>;

    template<class Field>
    using inner_decoder = kodo::fulcrum_inner_decoder<Field>;

}


TEST(TestFulcrum, inner)
{
    run_test_basic_api<
        encoder<fifi::binary8>,
        inner_decoder<fifi::binary> >(10, 1000);

    run_test_reuse<
        kodo::fulcrum_encoder<fifi::binary8>,
        kodo::fulcrum_inner_decoder<fifi::binary> >(10, 1000);

    run_test_reuse_incomplete<
        kodo::fulcrum_encoder<fifi::binary8>,
        kodo::fulcrum_inner_decoder<fifi::binary> >(10, 1000);

    run_test_initialize<
        kodo::fulcrum_encoder<fifi::binary8>,
        kodo::fulcrum_inner_decoder<fifi::binary> >(10, 1000);
}


// TEST(TestFulcrum, outer)
// {
//     test_basic_api<kodo::fulcrum_encoder<fifi::binary8>,
//         kodo::fulcrum_outer_decoder<fifi::binary8> >(10, 1000);

//     test_reuse<
//         kodo::fulcrum_encoder<fifi::binary8>,
//         kodo::fulcrum_outer_decoder<fifi::binary8> >(10, 1000);

//     test_reuse_incomplete<
//         kodo::fulcrum_encoder<fifi::binary8>,
//         kodo::fulcrum_outer_decoder<fifi::binary8> >(10, 1000);
// }

// TEST(TestFulcrum, combined)
// {
//     test_basic_api<kodo::fulcrum_encoder<fifi::binary8>,
//         kodo::fulcrum_combined_decoder<fifi::binary8> >(10, 1000);

//     test_reuse<
//         kodo::fulcrum_encoder<fifi::binary8>,
//         kodo::fulcrum_combined_decoder<fifi::binary8> >(10, 1000);

//     test_reuse_incomplete<
//         kodo::fulcrum_encoder<fifi::binary8>,
//         kodo::fulcrum_combined_decoder<fifi::binary8> >(10, 1000);
// }
