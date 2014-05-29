// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/set_symbols.hpp>
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
            void set_symbols(uint32_t symbols)
            {
                m_symbols = symbols;
            }

            uint32_t m_symbols;
        };

        struct dummy_two
        {
            uint32_t m_symbols;
        };

    }
}

TEST(TestSetSymbols, invoke)
{

    {
        // Check with a standard object
        kodo::dummy_one d;
        kodo::set_symbols(d, 10U);
        EXPECT_EQ(d.m_symbols, 10U);
    }

    {
        // Check that the code compiles even with an object that
        // does not have the set_symbols function
        kodo::dummy_two d;

        if(kodo::has_set_symbols<kodo::dummy_two>::value)
        {
            kodo::set_symbols(d, 10U);
            EXPECT_EQ(d.m_symbols, 10U);
        }
    }

    {
        // Check that the code compiles with a normal stack
        typedef kodo::full_rlnc_encoder<fifi::binary8> encoder_type;

        encoder_type::factory factory(10,10);
        kodo::set_symbols(factory, 5U);

        EXPECT_EQ(factory.symbols(), 5U);
    }

}
