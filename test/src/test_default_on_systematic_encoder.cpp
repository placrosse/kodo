// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_default_on_systematic_encoder.cpp Unit tests for the
///       kodo::default_on_systematic_encoder layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/default_on_systematic_encoder.hpp>
#include <kodo/default_off_systematic_encoder.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // Small helper struct which provides the API needed by the
        // default_on_systematic_encoder layer.
        struct dummy_layer
        {
        public:

            struct factory_base
            {
                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t max_symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t max_symbol_size() const
                {
                    return m_max_symbol_size;
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

            };

        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_symbols = the_factory.symbols();
                m_symbol_size = the_factory.max_symbol_size();
                m_is_symbol_uncoded.resize(m_symbols, false);
            }

            uint32_t encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
            {
                (void) symbol_data;

                m_systematic_index = symbol_index;
                m_encode_systematic = true;
                return 0;
            }

            uint32_t encode(uint8_t *symbol_data, uint8_t *symbol_header)
            {
                (void) symbol_data;
                (void) symbol_header;

                m_encode_full = true;
                return 0;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            bool is_symbol_uncoded(uint32_t index) const
            {
                (void) index;
                return m_is_symbol_uncoded[index];
            }

            uint32_t symbols_uncoded() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_is_symbol_uncoded[i];
                }
                return rank;
            }

            uint32_t header_size() const
            {
                return 10;
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size;
            }

            void reset()
            {
                m_systematic_index = m_symbols + 1;
                m_encode_systematic = false;
                m_encode_full = false;
            }

            uint32_t m_symbols;
            uint32_t m_symbol_size;

            std::vector<bool> m_is_symbol_uncoded;

            uint32_t m_systematic_index;

            bool m_encode_systematic;
            bool m_encode_full;
        };

        // Instantiate a stack containing the default_on_systematic_encoder
        class default_on_stack
            : public default_on_systematic_encoder<dummy_layer>
        {
        public:
            using factory = basic_factory<default_on_stack>;
        };

        // Instantiate a stack containing the default_on_systematic_encoder
        class default_off_stack
            : public default_off_systematic_encoder<dummy_layer>
        {
        public:
            using factory = basic_factory<default_off_stack>;
        };

    }

}

template<bool SystematicOn, class Stack>
inline void test()
{
    typedef Stack stack_type;
    typedef typename Stack::factory factory_type;

    uint32_t symbols = 10;
    uint32_t symbol_size = 100;

    stack_type stack;
    factory_type factory(symbols, symbol_size);

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.symbols(), symbols);
    EXPECT_EQ(stack.is_systematic_on(), SystematicOn);

    std::vector<uint8_t> header(stack.header_size());
    std::vector<uint8_t> symbol(stack.symbol_size());

    stack.set_systematic_on();

    stack.reset();

    // We encode but no data is available so we cannot code systematic
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);
    EXPECT_EQ(stack.systematic_count(), 0U);

    stack.reset();

    stack.m_is_symbol_uncoded[1] = true;

    EXPECT_EQ(stack.symbols_uncoded(), 1U);

    // We encode and there is one packet we should encode systematic
    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 1U);
    EXPECT_EQ(stack.systematic_count(), 1U);

    stack.reset();

    // We have sent one systematically so we encode full
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);

    stack.reset();

    stack.set_systematic_off();

    stack.m_is_symbol_uncoded[0] = true;

    // We have turned systematic off
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);

    stack.reset();

    stack.set_systematic_on();

    // We have turned systematic back on and we have one packet which has not
    // been sent systematically.
    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 0U);
}

/// Run the tests typical coefficients stack
TEST(TestDefaultOnSystematicEncoder, api)
{
    test<true, kodo::default_on_stack>();
}

/// Run the tests typical coefficients stack
TEST(TestDefaultOffSystematicEncoder, api)
{
    test<false, kodo::default_off_stack>();
}
