// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_partial_decoding_tracker.cpp Unit tests for the
///       kodo::partial_decoding_tracker

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/partial_decoding_tracker.hpp>
#include <kodo/symbol_decoding_status_tracker.hpp>
#include <kodo/symbol_decoding_status_counter.hpp>

namespace kodo
{

    // Small helper struct which provides the API needed by the
    // partial_decoding_tracker layer.
    struct dummy_rank
    {
        typedef uint32_t rank_type;

        template<class Factory>
        void inititalize(Factory& the_factory)
        {
            (void) the_factory;
        }

        void decode(uint8_t *payload)
        {
            (void) payload;
        }

        template<class Factory>
        void construct(Factory& the_factory)
        {
            (void) the_factory;
        }

        uint32_t rank() const
        {
            return m_rank;
        }

        uint32_t seen_encoder_rank() const
        {
            return m_seen_encoder_rank;
        }

        uint32_t symbols() const
        {
            return m_symbols;
        }


        uint32_t m_rank;
        uint32_t m_seen_encoder_rank;
        uint32_t m_symbols;
        uint32_t m_symbol_size;
    };

    // Instantiate a stack containing the partial_decoding_tracker
    class test_partial_stack
        : public // Payload API
                 // Codec Header API
                 // Symbol ID API
                 // Decoder API
                 partial_decoding_tracker<
                 symbol_decoding_status_counter<
                 symbol_decoding_status_tracker<
                 // Coefficient Storage API
                 // Storage API
                 // Finite Field API
                 // Factory API
                 // Final type
                 dummy_rank> > >
    { };

}

/// @todo: Finalize unit test
/// Run the tests typical coefficients stack
TEST(TestPartialDecodingTracker, test_partial_decoding)
{
    kodo::test_partial_stack stack;
    stack.m_rank = 0;
    stack.m_seen_encoder_rank = 0;

    EXPECT_FALSE(stack.is_partial_complete());

    stack.m_seen_encoder_rank = 1;
    EXPECT_FALSE(stack.is_partial_complete());

    stack.m_rank = 1;
    EXPECT_TRUE(stack.is_partial_complete());
}


