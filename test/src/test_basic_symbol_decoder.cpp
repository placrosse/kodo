// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_basic_symbol_decoder.cpp Unit tests for the
///       kodo::basic_symbol_decoder stack.

#include <gtest/gtest.h>

#include <kodo/basic_symbol_decoder.hpp>

#include "kodo_unit_test/helper_test_forward_decoding_stack.hpp"

namespace
{
    template<class Field>
    using decoder = kodo::basic_symbol_decoder<Field, kodo::disable_trace>;
}

TEST(TestBasicSymbolDecoder, decoder)
{
    // Here we will reutilize the test_forward_decoding_stack helper
    test_forward_decoding_stack<decoder>();
}
