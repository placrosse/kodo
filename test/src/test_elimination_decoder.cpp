// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>


#include <kodo/elimination_decoder.hpp>

template<class Field>
inline void run_test()
{
    typedef Field field_type;

    typedef kodo::elimination_decoder<field_type> decoder_type;

    uint32_t max_symbols = 8;
    uint32_t max_symbol_size = 100;

    typename decoder_type::factory factory(max_symbols, max_symbol_size);

    // The following two parameters are used to control the decoding
    // operations of the elimination decoder.
    // symbols as always refer to the number of symbols that this decoder
    // should decode. So when the rank reaches 4 the elimination decoder will
    // have finished its job.
    // symbols_offset is the number of symbols we will skip when looking in the
    // encoding vector. In this case we expect the encoder to produce a coded
    // packet with 2 + 4 coefficients. We will however not look at the first 2
    // coefficients when decoding.
    uint32_t symbols = 4;
    uint32_t symbols_offset = 2;

    factory.set_symbols(symbols);
    factory.set_elimination_offset(symbols_offset);

    auto d = factory.build();

    EXPECT_EQ(d->symbols(), symbols);
    EXPECT_EQ(d->elimination_offset(), symbols_offset);

    std::vector<uint8_t> coefficients(d->coefficient_vector_size());

    // Create an encoding vector looking like this: 01 1001 remember
    // we will only look at the last 4 coefficients
    std::fill(coefficients.begin(), coefficients.end(), 0);
    fifi::set_value<field_type>(&coefficients[0], 1, 1U);
    fifi::set_value<field_type>(&coefficients[0], 2, 1U);
    fifi::set_value<field_type>(&coefficients[0], 5, 1U);

    // Create a dummy symbol
    std::vector<uint8_t> symbol(d->symbol_size(), 'x');

    d->decode_symbol(&symbol[0], &coefficients[0]);

    // Decoding matrix status:
    // Row 1: 01 1001
    // Row 2: ?? ????
    // Row 3: ?? ????
    // Row 4: ?? ????

    EXPECT_EQ(d->rank(), 1U);

    EXPECT_TRUE(d->is_symbol_pivot(0U));
    EXPECT_TRUE(d->is_symbol_seen(0U));

    EXPECT_FALSE(d->is_complete());

    // Create an encoding vector looking like this: 11 0000
    std::fill(coefficients.begin(), coefficients.end(), 0);
    std::fill(symbol.begin(), symbol.end(), 'z');
    fifi::set_value<field_type>(&coefficients[0], 0, 1U);
    fifi::set_value<field_type>(&coefficients[0], 1, 1U);

    d->decode_symbol(&symbol[0], &coefficients[0]);

    // Rank should not have increased since we only had non-zero in the
    // offset - in fact the buffers should also be untouched
    EXPECT_EQ(d->rank(), 1U);

    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 0U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 1U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 2U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 3U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 4U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 5U), 0U);

    for(const auto& c : symbol)
        EXPECT_EQ(c, 'z');

    // Create an encoding vector looking like this: 10 1100

    std::fill(coefficients.begin(), coefficients.end(), 0);
    fifi::set_value<field_type>(&coefficients[0], 0, 1U);
    fifi::set_value<field_type>(&coefficients[0], 2, 1U);
    fifi::set_value<field_type>(&coefficients[0], 3, 1U);

    d->decode_symbol(&symbol[0], &coefficients[0]);

    // Decoding matrix status:
    // Row 1: 01 1001
    // Row 2: 11 0101
    // Row 3: ????
    // Row 4: ????

    EXPECT_EQ(d->rank(), 2U);

    // Create an encoding vector looking like this: 11 1101

    std::fill(coefficients.begin(), coefficients.end(), 0);
    fifi::set_value<field_type>(&coefficients[0], 0, 1U);
    fifi::set_value<field_type>(&coefficients[0], 1, 1U);
    fifi::set_value<field_type>(&coefficients[0], 2, 1U);
    fifi::set_value<field_type>(&coefficients[0], 3, 1U);
    fifi::set_value<field_type>(&coefficients[0], 5, 1U);

    d->decode_symbol(&symbol[0], &coefficients[0]);

    // Decoding matrix status:
    // Row 1: 00 1000 -> 10 0100
    // Row 2: 10 0100 -> 01 0001
    // Row 3: ????
    // Row 4: 01 0001

    EXPECT_EQ(d->rank(), 3U);

    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 0U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 1U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 2U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 3U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 4U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 5U), 1U);

    // Create an encoding vector looking like this: 001101

    std::fill(coefficients.begin(), coefficients.end(), 0);
    fifi::set_value<field_type>(&coefficients[0], 2, 1U);
    fifi::set_value<field_type>(&coefficients[0], 3, 1U);
    fifi::set_value<field_type>(&coefficients[0], 5, 1U);

    d->decode_symbol(&symbol[0], &coefficients[0]);

    // Decoding matrix status:
    // Row 1: 00 1000 -> 00 0101
    // Row 2: 10 0100 -> 10 0001
    // Row 3: ????
    // Row 4: 01 0001 -> 11 0000

    EXPECT_EQ(d->rank(), 3U);

    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 0U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 1U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 2U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 3U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 4U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 5U), 0U);

    // Create an encoding vector looking like this: 000011

    std::fill(coefficients.begin(), coefficients.end(), 0);
    fifi::set_value<field_type>(&coefficients[0], 4, 1U);
    fifi::set_value<field_type>(&coefficients[0], 5, 1U);

    d->decode_symbol(&symbol[0], &coefficients[0]);

    // Decoding matrix status:
    // Row 1: 00 1000
    // Row 2: 10 0100
    // Row 3: 01 0010
    // Row 4: 01 0001 -> 01 0010

    EXPECT_EQ(d->rank(), 4U);

    // Create an encoding vector looking like this: 111111

    std::fill(coefficients.begin(), coefficients.end(), 0);
    fifi::set_value<field_type>(&coefficients[0], 0, 1U);
    fifi::set_value<field_type>(&coefficients[0], 1, 1U);
    fifi::set_value<field_type>(&coefficients[0], 2, 1U);
    fifi::set_value<field_type>(&coefficients[0], 3, 1U);
    fifi::set_value<field_type>(&coefficients[0], 4, 1U);
    fifi::set_value<field_type>(&coefficients[0], 5, 1U);

    d->decode_symbol(&symbol[0], &coefficients[0]);

    // Decoding matrix status:
    // Row 1: 00 1000 -> 11 0111
    // Row 2: 10 0100 -> 01 0011
    // Row 3: 01 0010 -> 00 0001
    // Row 4: 01 0001 -> 01 0000

    EXPECT_EQ(d->rank(), 4U);

    // Check the output coefficients after passing the data to the decoder
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 0U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 1U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 2U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 3U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 4U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(&coefficients[0], 5U), 0U);

    // Check that the decoding matrix looks as expected
    // Row 1:
    auto c = d->coefficient_vector_values(0);

    EXPECT_EQ(fifi::get_value<field_type>(c, 0U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 1U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 2U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 3U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 4U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 5U), 0U);

    // Row 2:
    c = d->coefficient_vector_values(1);

    EXPECT_EQ(fifi::get_value<field_type>(c, 0U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 1U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 2U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 3U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 4U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 5U), 0U);

    // Row 3:
    c = d->coefficient_vector_values(2);

    EXPECT_EQ(fifi::get_value<field_type>(c, 0U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 1U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 2U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 3U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 4U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 5U), 0U);

    // Row 4:
    c = d->coefficient_vector_values(3);

    EXPECT_EQ(fifi::get_value<field_type>(c, 0U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 1U), 1U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 2U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 3U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 4U), 0U);
    EXPECT_EQ(fifi::get_value<field_type>(c, 5U), 1U);
}


TEST(TestEliminationDecoder, api)
{
    run_test<fifi::binary>();
    ///run_test<fifi::binary4>();
    run_test<fifi::binary8>();

    /// @todo enable this unit test
    //run_test<fifi::binary16>();
}
