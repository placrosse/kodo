// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_partial_decoding_tracker.cpp Unit tests for the
///       kodo::partial_decoding_tracker

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/partial_decoding_tracker.hpp>

namespace kodo
{

    // Small helper struct which provides the API needed by the
    // partial_decoding_tracker layer.
    struct dummy_layer
    {

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
        }

        void decode(uint8_t *payload)
        {
            m_payload = payload;
            m_symbols_decoded = m_symbols_decoded_new;
        }

        uint32_t symbols_decoded() const
        {
            return m_symbols_decoded;
        }

        uint8_t* m_payload;
        uint32_t m_symbols_decoded;
        uint32_t m_symbols_decoded_new;
    };

    // Instantiate a stack containing the partial_decoding_tracker
    class dummy_stack
        : public // Payload API
                 partial_decoding_tracker<
                 // Codec Header API
                 // Symbol ID API
                 // Decoder API
                 // Coefficient Storage API
                 // Storage API
                 // Finite Field API
                 // Factory API
                 // Final type
                 dummy_layer>
    { };

    struct dummy_factory
    { };

}


/// Run the tests typical coefficients stack
TEST(TestPartialDecodingTracker, test_partial_decoding)
{
    kodo::dummy_factory factory;
    kodo::dummy_stack stack;

    EXPECT_FALSE(stack.is_partial_complete());
    stack.initialize(factory);
    EXPECT_FALSE(stack.is_partial_complete());

    std::vector<uint8_t> payload(10);

    stack.m_symbols_decoded = 0;
    stack.m_symbols_decoded_new = 0;
    stack.decode(&payload[0]);

    EXPECT_FALSE(stack.is_partial_complete());

    stack.m_symbols_decoded_new = 1;
    stack.decode(&payload[0]);

    EXPECT_TRUE(stack.is_partial_complete());

    stack.m_symbols_decoded_new = 1;
    stack.decode(&payload[0]);

    EXPECT_FALSE(stack.is_partial_complete());

    stack.m_symbols_decoded_new = 10;
    stack.decode(&payload[0]);

    EXPECT_TRUE(stack.is_partial_complete());

    stack.initialize(factory);
    EXPECT_FALSE(stack.is_partial_complete());

}


