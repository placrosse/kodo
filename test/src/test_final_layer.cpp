// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_final_layer.cpp Unit tests for the
///       final_layer class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/final_layer.hpp>

TEST(TestFinalLayer, api)
{
    uint32_t symbols = 16;
    uint32_t symbol_size = 16;

    kodo::final_layer::factory_base factory(symbols, symbol_size);
    kodo::final_layer stack;

    stack.construct(factory);
    stack.initialize(factory);
}
