// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/set_symbol_size.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_one
        {
            void set_symbol_size(uint32_t symbol_size)
            {
                m_symbol_size = symbol_size;
            }

            uint32_t m_symbol_size;
        };

        struct dummy_two
        {
            uint32_t m_symbol_size;
        };

    }
}

TEST(TestSetSymbolSize, invoke)
{

    {
        // Check with a standard object
        kodo::dummy_one d;
        kodo::set_symbol_size(d, 10U);
        EXPECT_EQ(d.m_symbol_size, 10U);
    }

    {
        // Check that the code compiles even with an object that
        // does not have the set_symbol_size function
        kodo::dummy_two d;

        if(kodo::has_set_symbol_size<kodo::dummy_two>::value)
        {
            kodo::set_symbol_size(d, 10U);
            EXPECT_EQ(d.m_symbol_size, 10U);
        }
    }

    {
        // Check that the code compiles with a normal stack
        typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

        encoder_type::factory factory(10,10);
        kodo::set_symbol_size(factory, 5U);

        EXPECT_EQ(factory.symbol_size(), 5U);
    }

}
