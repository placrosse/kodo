// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_coefficient_info.cpp Unit tests for the
///       coefficient_info class

#include <cstdint>

#include <gtest/gtest.h>
#include <fifi/binary.hpp>
#include <fifi/binary8.hpp>
#include <kodo/coefficient_info.hpp>
#include <kodo/storage_block_info.hpp>
#include <kodo/pool_factory.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        /// Helper class to test the coefficient info API.
        template<class FieldType>
        class dummy_layer
        {
        public:

            typedef FieldType field_type;

        public:

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void)max_symbols;
                    (void)max_symbol_size;
                }

            };

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void)the_factory;
            }

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void)the_factory;
            }

        };

        template<class FieldType>
        class test_coefficient_info :
            public coefficient_info<
                   storage_block_info<
                   dummy_layer<
                   FieldType > > >
        {
            using factory = kodo::pool_factory<test_coefficient_info>;
        };
    }

}

TEST(TestCoefficientInfo, api)
{
    uint32_t symbols = 16;
    uint32_t symbols_size = 16;
    {
        using stack = kodo::test_coefficient_info<fifi::binary>;

        stack::factory f(symbols, symbols_size);

        stack info;
        info.construct(f);
        info.initialize(f);

        EXPECT_EQ(info.coefficient_vector_elements(), symbols);
        EXPECT_EQ(info.coefficient_vector_length(), 2U);
        EXPECT_EQ(info.coefficient_vector_size(), 2U);
    }

    {
        typedef kodo::test_coefficient_info<fifi::binary8> stack;

        stack::factory f(symbols, symbols_size);

        stack info;
        info.construct(f);
        info.initialize(f);

        EXPECT_EQ(info.coefficient_vector_elements(), symbols);
        EXPECT_EQ(info.coefficient_vector_length(), 16U);
        EXPECT_EQ(info.coefficient_vector_size(), 16U);
    }
}
