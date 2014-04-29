// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has.hpp>
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

namespace
{
    using decoder = kodo::full_rlnc_decoder<fifi::binary>;
}

TEST(TestHas, detect)
{
    bool test = kodo::has<decoder, kodo::deep_symbol_storage>::value;

    EXPECT_TRUE(test);//kodo::has<decoder,kodo::deep_symbol_storage>::value);

}
