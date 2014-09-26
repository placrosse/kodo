// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_rank_reader.cpp Unit tests for the
///       kodo::rank_reader layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/rank_reader.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // rank_reader layer.
        struct dummy_layer
        {
            typedef uint32_t rank_type;

            uint32_t rank() const
            {
                return m_rank;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }


            uint32_t m_rank;
        };

        struct dummy_factory
        { };

        // Instantiate a stack containing the rank_reader
        class dummy_stack
            : public rank_reader<
                     dummy_layer>
        { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestRankReader, api)
{
    typedef kodo::dummy_stack::rank_type rank_type;

    kodo::dummy_stack stack;
    kodo::dummy_factory factory;

    rank_type rank;
    std::vector<uint8_t> buffer(sizeof(rank_type));

    stack.initialize(factory);
    EXPECT_EQ(stack.remote_rank(), 0U);

    rank = 5;
    sak::big_endian::put<rank_type>(rank, &buffer[0]);

    stack.read_rank(&buffer[0]);
    EXPECT_EQ(stack.remote_rank(), 5U);

    stack.initialize(factory);
    EXPECT_EQ(stack.remote_rank(), 0U);

    rank = 453455;
    sak::big_endian::put<rank_type>(rank, &buffer[0]);

    stack.read_rank(&buffer[0]);
    EXPECT_EQ(stack.remote_rank(), 453455U);

    stack.initialize(factory);
    EXPECT_EQ(stack.remote_rank(), 0U);

}
