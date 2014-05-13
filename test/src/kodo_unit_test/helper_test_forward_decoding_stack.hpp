// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/fifi_utils.hpp>
#include "basic_api_test_helper.hpp"

// The function runs through a complete decoding scenario using
// manually constructed encoding vectors.
template<template <class> class Stack>
inline void test_forward_decoding_stack()
{
    typedef fifi::binary field_type;
    typename Stack<field_type>::factory f(5, 1600);

    auto d = f.build();

    EXPECT_EQ(d->coefficient_vector_size(), 1U);

    uint8_t coefficients[1];

    // Create an encoding vector looking like this: 01100
    coefficients[0] = 0;
    fifi::set_value<field_type>(coefficients, 1, 1U);
    fifi::set_value<field_type>(coefficients, 2, 1U);

    // Create a dummy symbol
    std::vector<uint8_t> symbol = random_vector(d->symbol_size());

    d->decode_symbol(&symbol[0], coefficients);

    EXPECT_EQ(d->rank(), 1U);
    EXPECT_TRUE(d->is_symbol_pivot(1U));
    EXPECT_TRUE(d->is_symbol_seen(1U));
    EXPECT_FALSE(d->is_complete());

    // Create an encoding vector looking like this: 11000
    coefficients[0] = 0;
    fifi::set_value<field_type>(coefficients, 0, 1U);
    fifi::set_value<field_type>(coefficients, 1, 1U);

    d->decode_symbol(&symbol[0], coefficients);

    EXPECT_EQ(d->rank(), 2U);
    EXPECT_TRUE(d->is_symbol_pivot(0));
    EXPECT_TRUE(d->is_symbol_pivot(1));
    EXPECT_TRUE(d->is_symbol_seen(0));
    EXPECT_TRUE(d->is_symbol_seen(1));
    EXPECT_FALSE(d->is_complete());

    // Create an systematic symbol for position 2
    d->decode_symbol(&symbol[0], 2U);

    EXPECT_EQ(d->rank(), 3U);
    EXPECT_TRUE(d->is_symbol_pivot(0));
    EXPECT_TRUE(d->is_symbol_pivot(1));
    EXPECT_TRUE(d->is_symbol_pivot(2));
    EXPECT_TRUE(d->is_symbol_seen(0));
    EXPECT_TRUE(d->is_symbol_seen(1));
    EXPECT_TRUE(d->is_symbol_uncoded(2));
    EXPECT_FALSE(d->is_complete());

    // Create an encoding vector looking like this: 11101
    coefficients[0] = 0;
    fifi::set_value<field_type>(coefficients, 0, 1U);
    fifi::set_value<field_type>(coefficients, 1, 1U);
    fifi::set_value<field_type>(coefficients, 2, 1U);
    fifi::set_value<field_type>(coefficients, 4, 1U);

    d->decode_symbol(&symbol[0], coefficients);

    EXPECT_EQ(d->rank(), 4U);
    EXPECT_TRUE(d->is_symbol_pivot(0));
    EXPECT_TRUE(d->is_symbol_pivot(1));
    EXPECT_TRUE(d->is_symbol_pivot(2));
    EXPECT_TRUE(d->is_symbol_pivot(4));
    EXPECT_TRUE(d->is_symbol_seen(0));
    EXPECT_TRUE(d->is_symbol_seen(1));
    EXPECT_TRUE(d->is_symbol_uncoded(2));
    EXPECT_TRUE(d->is_symbol_seen(4));
    EXPECT_FALSE(d->is_complete());

    // Create a _linear dependent_ encoding vector looking like this: 11101
    coefficients[0] = 0;
    fifi::set_value<field_type>(coefficients, 0, 1U);
    fifi::set_value<field_type>(coefficients, 1, 1U);
    fifi::set_value<field_type>(coefficients, 2, 1U);
    fifi::set_value<field_type>(coefficients, 4, 1U);

    d->decode_symbol(&symbol[0], coefficients);

    EXPECT_EQ(d->rank(), 4U);
    EXPECT_TRUE(d->is_symbol_pivot(0));
    EXPECT_TRUE(d->is_symbol_pivot(1));
    EXPECT_TRUE(d->is_symbol_pivot(2));
    EXPECT_TRUE(d->is_symbol_pivot(4));
    EXPECT_TRUE(d->is_symbol_seen(0));
    EXPECT_TRUE(d->is_symbol_seen(1));
    EXPECT_TRUE(d->is_symbol_uncoded(2));
    EXPECT_TRUE(d->is_symbol_seen(4));
    EXPECT_FALSE(d->is_complete());

    // Create an encoding vector looking like this: 01110
    coefficients[0] = 0;

    fifi::set_value<field_type>(coefficients, 1, 1U);
    fifi::set_value<field_type>(coefficients, 2, 1U);
    fifi::set_value<field_type>(coefficients, 3, 1U);

    d->decode_symbol(&symbol[0], coefficients);

    EXPECT_EQ(d->rank(), 5U);
    EXPECT_TRUE(d->is_symbol_pivot(0));
    EXPECT_TRUE(d->is_symbol_pivot(1));
    EXPECT_TRUE(d->is_symbol_pivot(2));
    EXPECT_TRUE(d->is_symbol_pivot(3));
    EXPECT_TRUE(d->is_symbol_pivot(4));
    EXPECT_TRUE(d->is_symbol_uncoded(0));
    EXPECT_TRUE(d->is_symbol_uncoded(1));
    EXPECT_TRUE(d->is_symbol_uncoded(2));
    EXPECT_TRUE(d->is_symbol_uncoded(3));
    EXPECT_TRUE(d->is_symbol_uncoded(4));
    EXPECT_TRUE(d->is_complete());
}
