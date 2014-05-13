// Copyright Steinwurf ApS 2011-2013.
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
            coefficient_storage_layers<
            // Storage API
            deep_storage_layers<enable_trace,
            // Finite Field API
            finite_field_layers<Field,
            // Factory API
            final_coder_factory_pool<
            // Final type
            test_forward_stack<Field>
            > > > > > > > >
        { };

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
            coefficient_storage_layers<
            // Storage API
            deep_storage_layers<enable_trace,
            // Finite Field API
            finite_field_layers<Field,
            // Factory API
            final_coder_factory_pool<
            // Final type
            test_forward_delayed_stack<Field>
            > > > > > > > > >
        { };
    }
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
