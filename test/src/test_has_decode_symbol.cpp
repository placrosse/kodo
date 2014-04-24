// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_decode_symbol.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_one
        {
            void decode_symbol(uint8_t*, uint32_t symbol_index);
            void decode_symbol(uint8_t*, uint8_t*);
        };

        struct dummy_two
        {
            void decode_symbol(uint8_t*, uint32_t symbol_index);
        };

        struct dummy_three
        {
            void decode_symbol(uint8_t*, uint8_t*);
        };


        struct dummy_parent : public dummy_one
        { };

        struct dummy_false
        { };

    }
}

TEST(TestHasDecode_Symbol, detect)
{
    EXPECT_FALSE(kodo::has_decode_symbol<uint32_t>::value);
    EXPECT_FALSE(kodo::has_decode_symbol<kodo::dummy_false>::value);
    EXPECT_TRUE(kodo::has_decode_symbol<kodo::dummy_one>::value);
    EXPECT_TRUE(kodo::has_decode_symbol<kodo::dummy_parent>::value);

    // False since they only provide one of the overloads
    EXPECT_FALSE(kodo::has_decode_symbol<kodo::dummy_two>::value);
    EXPECT_FALSE(kodo::has_decode_symbol<kodo::dummy_three>::value);

    typedef kodo::sliding_window_decoder<fifi::binary> decoder;

    EXPECT_TRUE(kodo::has_decode_symbol<decoder>::value);
}
