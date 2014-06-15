// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_payload_rank_reader.cpp Unit tests for the
///       kodo::payload_rank_reader layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/payload_rank_reader.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // payload_rank_reader layer.
        struct dummy_layer
        {
        public:

            typedef uint32_t rank_type;

        public:

            struct factory_base
            {
                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                uint32_t max_payload_size() const
                {
                    return m_max_payload_size;
                }

                uint32_t max_rank_size() const
                {
                    return sizeof(rank_type);
                }

                uint32_t m_max_payload_size;

            };

        public:

            void decode(uint8_t* payload)
            {
                m_payload = payload;
            }

            void read_rank(const uint8_t* rank)
            {
                (void) rank;
            }

            uint32_t payload_size() const
            {
                return m_payload_size;
            }

            uint32_t rank_size() const
            {
                return sizeof(rank_type);
            }

            uint32_t m_payload_size;
            const uint8_t* m_payload;
        };


        // Instantiate a stack containing the payload_rank_reader
        class dummy_stack
            : public payload_rank_reader<
                     dummy_layer>
        { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPayloadRankReader, api)
{
    typedef kodo::dummy_stack::rank_type rank_type;

    kodo::dummy_stack stack;
    kodo::dummy_stack::factory factory(10, 10);

    factory.m_max_payload_size = 10U;

    EXPECT_EQ(factory.max_payload_size(),
              factory.m_max_payload_size + sizeof(rank_type));

    stack.m_payload_size = 10U;

    EXPECT_EQ(stack.payload_size(),
              stack.m_payload_size + sizeof(rank_type));

    std::vector<uint8_t> buffer(stack.payload_size());

    stack.decode(&buffer[0]);

    EXPECT_EQ(stack.m_payload, &buffer[sizeof(rank_type)]);
}
