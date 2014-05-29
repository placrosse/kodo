// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_sparse_uniform_generator.hpp Unit tests for the sparse uniform
///       coefficient generators

#include <fifi/is_binary.hpp>

#include <kodo/perpetual_generator.hpp>

#include "kodo_unit_test/coefficient_generator_helper.hpp"

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // Perpetual generator
        template<class Field>
        class perpetual_generator_stack :
            public perpetual_generator<
                   fake_codec_layer<
                   coefficient_info<
                   fake_symbol_storage<
                   storage_block_info<
                   finite_field_info<Field,
                   final_coder_factory<
                   perpetual_generator_stack<Field>
                   > > > > > > >
        { };

        template<class Field>
        class perpetual_generator_stack_pool :
            public perpetual_generator<
                   fake_codec_layer<
                   coefficient_info<
                   fake_symbol_storage<
                   storage_block_info<
                   finite_field_info<Field,
                   final_coder_factory_pool<
                   perpetual_generator_stack_pool<Field>
                   > > > > > > >
        { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPerpetualGenerator, test_perpetual_generator_stack)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    // API tests:
    run_test<
        kodo::perpetual_generator_stack,
        api_generate>(symbols, symbol_size);

    run_test<
        kodo::perpetual_generator_stack_pool,
        api_generate>(symbols, symbol_size);
}
