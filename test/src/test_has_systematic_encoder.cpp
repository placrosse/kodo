// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_has_systematic_encoder.cpp Unit tests for the
///       has_systematic_encoder class

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/has_systematic_encoder.hpp>
#include <kodo/default_on_systematic_encoder.hpp>
#include <kodo/default_off_systematic_encoder.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/rlnc/on_the_fly_codes.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        class dummy
        {
        public:

            typedef uint8_t field_type;
            typedef uint8_t value_type;

        public:

            class factory
            {
            public:

                /// @copydoc layer::factory::factory(uint32_t,uint32_t)
                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                uint32_t max_header_size() const
                {
                    return 0;
                }
            };

        public:

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }

            bool in_systematic_phase() const
            {
                return false;
            }

            uint32_t header_size() const
            {
                return 0;
            }

            uint32_t next_systematic_symbol() const
            {
                return 0;
            }

            void encode_symbol(uint8_t*, uint32_t)
            { }

            void encode_symbol(uint8_t*, uint8_t*)
            { }

        };

        struct dummy_has_true : public systematic_encoder<dummy>
        { };

        struct dummy_has_false : public systematic_encoder<dummy>
        { };

        struct dummy_default_on : public default_on_systematic_encoder<dummy>
        { };

        struct dummy_default_off : public default_off_systematic_encoder<dummy>
        { };

    }
}

TEST(TestHasSystematicEncoder, detect)
{
    EXPECT_FALSE(kodo::has_systematic_encoder<uint32_t>::value);
    EXPECT_FALSE(kodo::has_systematic_encoder<kodo::dummy>::value);
    EXPECT_TRUE(kodo::has_systematic_encoder<kodo::dummy_has_true>::value);
    EXPECT_TRUE(kodo::has_systematic_encoder<kodo::dummy_has_false>::value);
}
