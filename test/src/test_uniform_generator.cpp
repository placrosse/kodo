// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file coefficient_generator.hpp Unit tests for the uniform coefficient
///       generators

#include "kodo_unit_test/coefficient_generator_helper.hpp"

#include <kodo/pivot_aware_generator.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Uniform generator
        template<class Field>
        class uniform_generator_stack :
            public uniform_generator<
                   pivot_aware_generator<
                   fake_codec_layer<
                   coefficient_info<
                   fake_symbol_storage<
                   storage_block_length<
                   storage_block_size<
                   finite_field_info<Field,
                   final_layer
                   > > > > > > > >
        {
        public:
            using factory = basic_factory<uniform_generator_stack>;
        };

        template<class Field>
        class uniform_generator_stack_pool :
            public uniform_generator<
                   pivot_aware_generator<
                   fake_codec_layer<
                   coefficient_info<
                   fake_symbol_storage<
                   storage_block_length<
                   storage_block_size<
                   finite_field_info<Field,
                   final_layer
                   > > > > > > > >
        {
        public:
            using factory = basic_factory<uniform_generator_stack_pool>;
        };
    }
}

/// Run the tests typical coefficients stack
TEST(TestCoefficientGenerator, test_uniform_generator_stack)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    // API tests:
    run_test<
        kodo::uniform_generator_stack,
        api_generate>(symbols, symbol_size);

    run_test<
        kodo::uniform_generator_stack_pool,
        api_generate>(symbols, symbol_size);
}
