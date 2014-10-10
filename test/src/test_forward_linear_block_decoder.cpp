// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_forward_linear_block_decoder.cpp Unit tests for the
///       kodo::forward_linear_block_decoder

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/forward_linear_block_decoder.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>

#include <kodo/trace_linear_block_decoder.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"
#include "kodo_unit_test/helper_test_forward_decoding_stack.hpp"

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        template<class Field>
        class test_forward_stack : public
            // Payload API
            // Codec Header API
            // Symbol ID API
            // Decoder API
            trace_linear_block_decoder<enable_trace,
            forward_linear_block_decoder<
            symbol_decoding_status_counter<
            symbol_decoding_status_tracker<
            // Coefficient Storage API
            coefficient_value_access<
            coefficient_storage<
            coefficient_info<
            // Storage API
            deep_symbol_storage<
            storage_bytes_used<
            storage_block_length<
            storage_block_size<
            // Finite Field API
            finite_field_math<typename fifi::default_field<Field>::type,
            finite_field_info<Field,
            // Final Layer
            final_layer
            > > > > > > > > > > > > >
        {
        public:
            using factory = basic_factory<test_forward_stack>;
        };

        template<class Field>
        class test_forward_delayed_stack : public
            // Payload API
            // Codec Header API
            // Symbol ID API
            // Decoder API
            trace_linear_block_decoder<enable_trace,
            linear_block_decoder_delayed<
            forward_linear_block_decoder<
            symbol_decoding_status_counter<
            symbol_decoding_status_tracker<
            // Coefficient Storage API
            coefficient_value_access<
            coefficient_storage<
            coefficient_info<
            // Storage API
            deep_symbol_storage<
            storage_bytes_used<
            storage_block_length<
            storage_block_size<
            // Finite Field API
            finite_field_math<typename fifi::default_field<Field>::type,
            finite_field_info<Field,
            // Final Layer
            final_layer
            > > > > > > > > > > > > > >
        {
        public:
            using factory = basic_factory<test_forward_delayed_stack>;
        };
    }
}

template<template <class> class Stack>
void test_forward_stack()
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
    // d->print_decoder_state(std::cout);

    EXPECT_EQ(d->rank(), 1U);

    EXPECT_TRUE(d->is_symbol_pivot(1U));

    EXPECT_TRUE(d->is_symbol_seen(1U));

    EXPECT_FALSE(d->is_complete());

    // Create an encoding vector looking like this: 11000
    coefficients[0] = 0;
    fifi::set_value<field_type>(coefficients, 0, 1U);
    fifi::set_value<field_type>(coefficients, 1, 1U);

    d->decode_symbol(&symbol[0], coefficients);
    // d->print_decoder_state(std::cout);

    EXPECT_EQ(d->rank(), 2U);

    EXPECT_TRUE(d->is_symbol_pivot(0));
    EXPECT_TRUE(d->is_symbol_pivot(1));

    EXPECT_TRUE(d->is_symbol_seen(0));
    EXPECT_TRUE(d->is_symbol_seen(1));

    EXPECT_FALSE(d->is_complete());

    // Create an systematic symbol for position 2
    d->decode_symbol(&symbol[0], 2U);
    // d->print_decoder_state(std::cout);

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
    // d->print_decoder_state(std::cout);

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
    // d->print_decoder_state(std::cout);

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
    // d->print_decoder_state(std::cout);

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

/// Run the tests typical coefficients stack
TEST(TestForwardLinearBlockDecoder, test_decoder)
{
    test_forward_decoding_stack<kodo::test_forward_stack>();
}

TEST(TestForwardLinearBlockDecoder, test_decoder_delayed)
{
    test_forward_decoding_stack<kodo::test_forward_delayed_stack>();
}
