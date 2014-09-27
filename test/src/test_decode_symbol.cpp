// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <sstream>

#include <gtest/gtest.h>

#include <kodo/decode_symbol.hpp>
#include <kodo/rlnc/sliding_window_encoder.hpp>
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
            void decode_symbol(uint8_t* symbol_data, uint32_t symbol_index)
            {
                m_symbol_data = symbol_data;
                m_symbol_index = symbol_index;
            }

            void decode_symbol(uint8_t* symbol_data,
                               uint8_t* symbol_coefficients)
            {
                m_symbol_data = symbol_data;
                m_symbol_coefficients = symbol_coefficients;
            }

            void init()
            {
                m_symbol_data = nullptr;
                m_symbol_coefficients = nullptr;
                m_symbol_index = 0U;
            }

            uint8_t* m_symbol_data;
            uint8_t* m_symbol_coefficients;
            uint32_t m_symbol_index;
        };

        struct dummy_two
        {
            void decode_symbol(uint8_t*, uint32_t symbol_index);
        };

        struct dummy_three
        {
            void decode_symbol(uint8_t*, uint8_t*);
        };

    }
}

TEST(TestDecodeSymbol, invoke)
{

    uint8_t* data = (uint8_t*) 0xbeef;
    uint8_t* coefficients = (uint8_t*) 0xdead;
    uint32_t index = 10;

    // Check with a standard object
    {
        kodo::dummy_one d;

        d.init();
        kodo::decode_symbol(d, data, coefficients);

        EXPECT_EQ(d.m_symbol_data, data);
        EXPECT_EQ(d.m_symbol_coefficients, coefficients);
        EXPECT_EQ(d.m_symbol_index, 0U);

        d.init();
        kodo::decode_symbol(d, data, index);

        EXPECT_EQ(d.m_symbol_data, data);
        EXPECT_EQ(d.m_symbol_coefficients, nullptr);
        EXPECT_EQ(d.m_symbol_index, index);

    }

    // Check that the code compiles even with an object that
    // does not have both the decode_symbol functions

    {
        kodo::dummy_two d;

        if(kodo::has_decode_symbol<kodo::dummy_two>::value)
        {
            kodo::decode_symbol(d, data, coefficients);
            kodo::decode_symbol(d, data, index);
        }
    }

    {
        kodo::dummy_three d;

        if(kodo::has_decode_symbol<kodo::dummy_three>::value)
        {
            kodo::decode_symbol(d, data, coefficients);
            kodo::decode_symbol(d, data, index);
        }
    }

}
