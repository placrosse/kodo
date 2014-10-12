// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>

#include <kodo/rlnc/fulcrum_outer_symbol_mapper.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        class dummy_layer
        {
        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                m_construct_called = true;
                (void) the_factory;
            }

            void map_to_outer(const uint8_t* inner_coefficients,
                uint8_t* outer_coefficients)
            {
                m_map_to_outer1_called = true;
                (void)inner_coefficients;
                (void)outer_coefficients;
            }

            void map_to_outer(uint32_t inner_symbol,
                uint8_t* outer_coefficients)
            {
                m_map_to_outer2_called = true;
                (void)inner_symbol;
                (void)outer_coefficients;
            }

            void decode_symbol(uint8_t *symbol_data,
                uint8_t *symbol_coefficients)
            {
                m_decode_symbol1_called = true;
                (void)symbol_data;
                (void)symbol_coefficients;
            }

            void decode_symbol(uint8_t *symbol_data,
                uint32_t symbol_index)
            {
                m_decode_symbol2_called = true;
                (void)symbol_data;
                (void)symbol_index;
            }

            uint32_t inner_symbols()
            {
                return 10;
            }

            void set_outer_systematic(bool outer_systematic)
            {
                m_outer_systematic = outer_systematic;
            }

            bool is_outer_systematic()
            {
                return m_outer_systematic;
            }

            uint32_t symbols()
            {
                return 10;
            }

            class factory_base
            {
            public:

                factory_base(uint32_t max_coefficient_vector_size)
                    : m_max_coefficient_vector_size(max_coefficient_vector_size)
                { }

                uint32_t max_coefficient_vector_size() const
                {
                    return m_max_coefficient_vector_size;
                }

                uint32_t m_max_coefficient_vector_size;
            };

            bool m_construct_called = false;
            bool m_map_to_outer1_called = false;
            bool m_map_to_outer2_called = false;
            bool m_decode_symbol1_called = false;
            bool m_decode_symbol2_called = false;

        private:
            bool m_outer_systematic = true;
        };

        class dummy_stack
            : public fulcrum_outer_symbol_mapper<dummy_layer>
        { };
    }
}

TEST(TestFulcrumOuterSymbolMapper, api)
{
    uint32_t max_coefficient_vector_size = 10;

    typedef kodo::dummy_stack test_stack;

    test_stack::factory_base factory(max_coefficient_vector_size);

    EXPECT_EQ(factory.max_coefficient_vector_size(),
        max_coefficient_vector_size);

    test_stack stack;

    EXPECT_FALSE(stack.m_construct_called);
    stack.construct(factory);
    EXPECT_TRUE(stack.m_construct_called);

    std::vector<uint8_t> symbol_data(10);
    std::vector<uint8_t> coefficients(10);
    uint32_t index = 5;


    EXPECT_FALSE(stack.m_map_to_outer1_called);
    EXPECT_FALSE(stack.m_decode_symbol1_called);
    stack.decode_symbol(symbol_data.data(), coefficients.data());
    EXPECT_TRUE(stack.m_map_to_outer1_called);
    EXPECT_TRUE(stack.m_decode_symbol1_called);

    EXPECT_FALSE(stack.m_decode_symbol2_called);
    stack.decode_symbol(symbol_data.data(), index);
    EXPECT_TRUE(stack.m_decode_symbol2_called);

    stack.set_outer_systematic(false);

    EXPECT_FALSE(stack.m_map_to_outer2_called);
    stack.decode_symbol(symbol_data.data(), index);
    EXPECT_TRUE(stack.m_map_to_outer2_called);
}
