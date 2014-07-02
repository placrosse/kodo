// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>
#include <fifi/binary.hpp>
#include <fifi/binary8.hpp>
#include <kodo/elimination_coefficient_info.hpp>
#include <kodo/storage_block_size.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        /// Helper class to test coefficient value access layer
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
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t max_symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t symbols() const
                {
                    return m_symbols;
                }

                uint32_t elimination_offset() const
                {
                    return m_elimination_offset;
                }

            public:

                uint32_t m_symbols;
                uint32_t m_elimination_offset;
                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;
            };

        public:

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }

        };

        /// Helper stack for testing the coefficient access layer
        template<class FieldType>
        class dummy_stack :
            public elimination_coefficient_info<
                   dummy_layer<FieldType> >
        {
        };
    }
}

TEST(TestEliminationCoefficientInfo, api)
{
    {
        typedef fifi::binary field_type;
        typedef kodo::dummy_stack<field_type> stack_type;

        uint32_t max_symbols = 18;
        uint32_t max_symbol_size = 1300;

        stack_type::factory_base factory(max_symbols, max_symbol_size);
        factory.m_symbols = 10;
        factory.m_elimination_offset = 6;

        EXPECT_EQ(factory.elimination_offset(), factory.m_elimination_offset);
        EXPECT_EQ(factory.symbols(), factory.m_symbols);

        stack_type stack;
        stack.initialize(factory);

        // Check that we use uint8_t for the binary field, we
        // otherwise we need to adjust the calculation of the
        // coefficient_vector_length()
        static_assert(std::is_same<field_type::value_type, uint8_t>::value,
                      "The binary field uses uint8_t as value type");

        // If uint8_t is the value type and we are in binary we need
        // two uint8_t for 10 symbols + 6 in offset
        EXPECT_EQ(stack.coefficient_vector_length(), 2U);

        // The number of bytes needed are also 2
        EXPECT_EQ(stack.coefficient_vector_size(), 2U);

        factory.m_symbols = 5;
        factory.m_elimination_offset = 0;

        stack.initialize(factory);

        // If uint8_t is the value type and we are in binary we need
        // two uint8_t for 5 symbols + 0 in offset
        EXPECT_EQ(stack.coefficient_vector_length(), 1U);

        // The number of bytes needed are also 1
        EXPECT_EQ(stack.coefficient_vector_size(), 1U);

    }

    {
        typedef fifi::binary8 field_type;
        typedef kodo::dummy_stack<field_type> stack_type;

        uint32_t max_symbols = 18;
        uint32_t max_symbol_size = 1300;

        stack_type::factory_base factory(max_symbols, max_symbol_size);
        factory.m_symbols = 10;
        factory.m_elimination_offset = 6;

        EXPECT_EQ(factory.elimination_offset(), factory.m_elimination_offset);
        EXPECT_EQ(factory.symbols(), factory.m_symbols);

        stack_type stack;
        stack.initialize(factory);

        // Check that we use uint8_t for the binary8 field, we
        // otherwise we need to adjust the calculation of the
        // coefficient_vector_length()
        static_assert(std::is_same<field_type::value_type, uint8_t>::value,
                      "The binary field uses uint8_t as value type");

        // If uint8_t is the value type and we are in binary8 we need
        // ten uint8_t for 10 symbols + 6 uint8_t for the offset
        EXPECT_EQ(stack.coefficient_vector_length(), 16U);
        EXPECT_EQ(stack.coefficient_vector_size(), 16U);

        factory.m_symbols = 5;
        factory.m_elimination_offset = 0;

        stack.initialize(factory);

        EXPECT_EQ(stack.coefficient_vector_length(), 5U);
        EXPECT_EQ(stack.coefficient_vector_size(), 5U);

    }

}
