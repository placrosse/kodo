// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_remote_rank_writer.cpp Unit tests for the
///       kodo::remote_rank_writer layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/remote_rank_writer.hpp>

#include "helper_test_rank_writer.hpp"

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // remote_rank_writer layer.
        struct dummy_layer
        {
            typedef uint32_t rank_type;

            uint32_t remote_rank() const
            {
                return m_rank;
            }

            uint32_t m_rank;
        };

        // Instantiate a stack containing the remote_rank_writer
        class dummy_stack
            : public remote_rank_writer<
                     dummy_layer>
          { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestRemoteRankWriter, api)
{
    test_rank_writer<kodo::dummy_stack>();
}
