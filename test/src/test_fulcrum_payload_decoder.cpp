// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <kodo/rlnc/fulcrum_payload_decoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        class dummy_main_stack
        {
        public:

            typedef fifi::binary8 field_type;
            typedef field_type::value_type value_type;

            class factory
            {
            public:
            };

            void decode_symbol(uint8_t* symbol_data, uint32_t symbol_index)
            {
                (void) symbol_data;
                (void) symbol_index;
                m_systematic = true;
            }

            void decode_symbol(uint8_t* symbol_data,
                               uint8_t* symbol_coefficients)
            {
                (void) symbol_data;
                (void) symbol_coefficients;
                m_non_systematic = true;
            }

            bool m_systematic;
            bool m_non_systematic;
        };
    }
}

TEST(TestFulcrumPayloadDecoder, api)
{

    typedef kodo::fulcrum_payload_decoder<kodo::dummy_main_stack> test_stack;
    typedef kodo::dummy_main_stack main_stack;

    uint32_t symbols = 10;
    uint32_t symbol_size = 10;

    // Create the two main objects
    main_stack main;
    main.m_systematic = false;
    main.m_non_systematic = false;

    main_stack::factory main_factory;

    test_stack::factory factory(symbols, symbol_size);
    factory.set_main_factory(&main_factory);
    factory.set_main_stack(&main);

    // Since the payload decoder expect a binary full vector encoder
    // we know the max payload must be 2 bytes for the 10 bit
    // coefficients and 10 bytes for symbol + 1 byte for the
    // systematic byte + 4 bytes for the systematic index. If this
    // change we need to update this unit test.
    EXPECT_EQ(factory.max_coefficient_vector_size(), 2U);
    EXPECT_EQ(factory.max_coefficient_vectors(), symbols);
    EXPECT_EQ(factory.max_symbol_size(), symbol_size);
    EXPECT_EQ(factory.max_symbols(), symbols);

    auto size = factory.max_payload_size();
    EXPECT_EQ(size, 17U);

    auto stack = factory.build();

    std::vector<uint8_t> payload(size, 0);

    // Since the buffer is all zeros the systematic flag should also
    // be off so we expect a non-systematic call

    stack->decode(&payload[0]);

    EXPECT_EQ(main.m_systematic, false);
    EXPECT_EQ(main.m_non_systematic, true);

    main.m_non_systematic = false;

    // The systematic flag is the first thing after the symbol so at byte 11
    payload[10] = 0xff;

    stack->decode(&payload[0]);

    EXPECT_EQ(main.m_systematic, true);
    EXPECT_EQ(main.m_non_systematic, false);
}


