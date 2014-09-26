// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_payload_rank_recoder.cpp Unit tests

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/payload_rank_recoder.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // Dummy layer needed to instantiate the payload_rank_decoder layer
        class dummy_layer
        {
        public:

            // The data type of the rank
            typedef uint32_t rank_type;

        public:

            // The dummy factory object
            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                uint32_t max_payload_size() const
                {
                    return 0;
                }

                uint32_t max_rank_size() const
                {
                    return sizeof(rank_type);
                }

            };

        public:

            uint32_t encode(uint8_t* payload)
            {
                m_payload = payload;
                return 0;
            }

            rank_type remote_rank() const
            {
                return m_remote_rank;
            }

            uint32_t payload_size() const
            {
                return 0;
            }

            uint32_t rank_size() const
            {
                return sizeof(rank_type);
            }

        public:

            uint8_t* m_payload;
            uint32_t m_remote_rank;

        };

        class test_stack : public payload_rank_recoder<dummy_layer>
        {
        public:
            using factory = basic_factory<test_stack>;
        };
    }
}

TEST(TestPayloadRankRecoder, factory)
{
    kodo::test_stack::factory factory(10, 10);
    EXPECT_EQ(factory.max_payload_size(), sizeof(kodo::test_stack::rank_type));
}

TEST(TestPayloadRankRecoder, stack)
{
    typedef kodo::test_stack::rank_type rank_type;

    kodo::test_stack stack;
    EXPECT_EQ(stack.payload_size(), sizeof(rank_type));

    std::vector<uint8_t> payload(stack.payload_size());

    stack.m_remote_rank = 0U;

    uint32_t bytes_used = stack.encode(&payload[0]);
    EXPECT_EQ(bytes_used, sizeof(rank_type));
    EXPECT_EQ(stack.m_payload, &payload[4]);

    rank_type rank = sak::big_endian::get<rank_type>(&payload[0]);
    EXPECT_EQ(rank, stack.m_remote_rank);

    stack.m_remote_rank = 4U;

    bytes_used = stack.encode(&payload[0]);
    EXPECT_EQ(bytes_used, sizeof(rank_type));
    EXPECT_EQ(stack.m_payload, &payload[4]);

    rank = sak::big_endian::get<rank_type>(&payload[0]);
    EXPECT_EQ(rank, stack.m_remote_rank);

    stack.m_remote_rank = 1U;

    bytes_used = stack.encode(&payload[0]);
    EXPECT_EQ(bytes_used, sizeof(rank_type));
    EXPECT_EQ(stack.m_payload, &payload[4]);

    rank = sak::big_endian::get<rank_type>(&payload[0]);
    EXPECT_EQ(rank, stack.m_remote_rank);

    stack.m_remote_rank = 0U;

    bytes_used = stack.encode(&payload[0]);
    EXPECT_EQ(bytes_used, sizeof(rank_type));
    EXPECT_EQ(stack.m_payload, &payload[4]);

    rank = sak::big_endian::get<rank_type>(&payload[0]);
    EXPECT_EQ(rank, stack.m_remote_rank);
}
