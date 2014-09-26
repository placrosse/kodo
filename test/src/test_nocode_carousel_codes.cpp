// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_carousel_codes.cpp Unit tests for the carousel nocode scheme

#include <ctime>

#include <gtest/gtest.h>

#include <kodo/nocode/nocode_carousel_codes.hpp>

#include "kodo_unit_test/helper_test_basic_api.hpp"

namespace
{
    using encoder = kodo::nocode_carousel_encoder<kodo::disable_trace>;
    using decoder = kodo::nocode_carousel_decoder<kodo::disable_trace>;
}

static void test_coders(uint32_t symbols, uint32_t symbol_size)
{
    run_test_basic_api<encoder,decoder>(symbols, symbol_size);
}

TEST(TestNoCodeCarouselCodes, basic_api)
{
    test_coders(32, 1600);
    test_coders(1, 1600);

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_coders(symbols, symbol_size);
}
