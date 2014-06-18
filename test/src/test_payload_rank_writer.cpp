// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_payload_rank_writer.cpp Unit tests for the
///       kodo::payload_rank_writer layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/payload_rank_writer.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // payload_rank_writer layer.
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

            uint32_t encode(uint8_t* payload)
            {
                m_payload = payload;
                return rank_size();
            }

            void write_rank(uint8_t* rank)
            {
                m_rank = rank;
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
            const uint8_t* m_rank;
        };


        // Instantiate a stack containing the payload_rank_writer
        class dummy_stack
            : public payload_rank_writer<
                     dummy_layer>
        {
        public:
            using factory = basic_factory<dummy_stack>;
        };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPayloadRankWriter, api)
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

    stack.encode(&buffer[0]);

    EXPECT_EQ(stack.m_rank, &buffer[0]);
    EXPECT_EQ(stack.m_payload, &buffer[sizeof(rank_type)]);
}
