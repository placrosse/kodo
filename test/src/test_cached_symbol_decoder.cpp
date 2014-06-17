// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_cache_decode_symbol.cpp Unit tests for the
///       cache_decode_symbol layer

#include "kodo_unit_test/basic_api_test_helper.hpp"

#include <kodo/cache_decode_symbol.hpp>
#include <kodo/empty_decoder.hpp>
#include <kodo/storage_block_info.hpp>
#include <kodo/finite_field_info.hpp>
#include <kodo/final_coder_factory.hpp>
#include <kodo/coefficient_info.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        /// Test stack for the cache_decode_symbol
        template<class Field>
        class cache_decode_symbol_stack : public
            cache_decode_symbol<
            empty_decoder<
            coefficient_info<
            storage_block_info<
            finite_field_info<Field,
            final_coder_factory<
            cache_decode_symbol_stack<Field>
            > > > > > >
        {
        public:
            using factory = pool_factory<cache_decode_symbol_stack<Field>>;
        };
    }
}

template<class Stack>
void test_cache_decode_symbol(uint32_t symbols, uint32_t symbol_size)
{
    typename Stack::factory factory(symbols, symbol_size);
    auto stack = factory.build();

    EXPECT_EQ(factory.symbol_size(), symbol_size);
    EXPECT_EQ(stack->symbol_size(), symbol_size);

    std::vector<uint8_t> data_in =
        random_vector(symbol_size);

    std::vector<uint8_t> coeff_in =
        random_vector(stack->coefficient_vector_size());

    stack->decode_symbol(&data_in[0], &coeff_in[0]);

    EXPECT_EQ(stack->cached_symbol_coded(), true);

    auto data_storage_in = sak::storage(data_in);
    auto coeff_storage_in = sak::storage(coeff_in);

    auto data_storage_out = sak::storage(stack->cached_symbol_data(),
                                         stack->symbol_size());

    auto coeff_storage_out = sak::storage(stack->cached_symbol_coefficients(),
                                          stack->coefficient_vector_size());

    EXPECT_TRUE(sak::equal(data_storage_in, data_storage_out));
    EXPECT_TRUE(sak::equal(coeff_storage_in, coeff_storage_out));

    data_in = random_vector(symbol_size);

    // The maximum index is symbols - 1 e.g. if we have 5 symbols
    // the indicies are {0,1,2,3,4} i.e. the maximum is 4
    assert(symbols > 0);
    uint32_t random_index = rand() % symbols;

    stack->decode_symbol(&data_in[0], random_index);

    EXPECT_EQ(stack->cached_symbol_coded(), false);

    data_storage_in = sak::storage(data_in);
    data_storage_out = sak::storage(stack->cached_symbol_data(),
                                    stack->symbol_size());

    EXPECT_TRUE(sak::equal(data_storage_in, data_storage_out));
    EXPECT_EQ(random_index, stack->cached_symbol_index());
}

/// Run the tests typical coefficients stack
TEST(TestCachedSymbolDecoder, test_cache_decode_symbol)
{

    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    {
        test_cache_decode_symbol<
            kodo::cache_decode_symbol_stack<fifi::binary> >(
                symbols, symbol_size);
    }

    {
        test_cache_decode_symbol<
            kodo::cache_decode_symbol_stack<fifi::binary8> >(
                symbols, symbol_size);
    }

    {
        test_cache_decode_symbol<
            kodo::cache_decode_symbol_stack<fifi::binary16> >(
                symbols, symbol_size);
    }

}
