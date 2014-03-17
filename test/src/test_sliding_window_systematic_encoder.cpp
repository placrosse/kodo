// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_sliding_window_systematic_encoder.cpp Unit tests for the
///       kodo::sliding_window_systematic_encoder layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/rlnc/sliding_window_systematic_encoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // sliding_window_systematic_encoder layer.
        struct dummy_layer
        {
        public:

            struct factory
            {
                /// @copydoc layer::factory::factory(uint32_t,uint32_t)
                factory(uint32_t max_symbols, uint32_t max_symbol_size)
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
                m_is_symbol_pivot.resize(m_symbols, false);
                m_remote_is_symbol_pivot.resize(m_symbols, false);
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

            bool is_symbol_pivot(uint32_t index) const
            {
                (void) index;
                return m_is_symbol_pivot[index];
            }

            uint32_t rank() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_is_symbol_pivot[i];
                }
                return rank;
            }

            bool remote_is_symbol_pivot(uint32_t index) const
            {
                (void) index;
                return m_remote_is_symbol_pivot[index];
            }

            uint32_t remote_rank() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_remote_is_symbol_pivot[i];
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

            std::vector<bool> m_is_symbol_pivot;
            std::vector<bool> m_remote_is_symbol_pivot;


            uint32_t m_systematic_index;

            bool m_encode_systematic;
            bool m_encode_full;
        };

        // Instantiate a stack containing the sliding_window_systematic_encoder
        class dummy_stack
            : public sliding_window_systematic_encoder<dummy_layer>
        { };

    }

}


/// Run the tests typical coefficients stack
TEST(TestSlidingWindowSystematicEncoder, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 100;

    kodo::dummy_stack stack;
    kodo::dummy_stack::factory factory(symbols, symbol_size);

    stack.construct(factory);
    stack.initialize(factory);

    EXPECT_EQ(stack.symbols(), symbols);
    EXPECT_EQ(stack.is_systematic_on(), true);

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

    // Simulate that we have one packet available
    stack.m_is_symbol_pivot[1] = true;

    EXPECT_EQ(stack.rank(), 1U);

    // We encode and there is one packet we should encode systematic
    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 1U);
    EXPECT_EQ(stack.systematic_count(), 1U);

    stack.reset();

    // We have sent one systematically but the difference between the
    // "local" remote rank is 1 in which case we send the packet
    // systematically again
    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 1U);
    EXPECT_EQ(stack.systematic_count(), 1U);

    stack.reset();

    stack.m_is_symbol_pivot[0] = true;

    // We have added one more packet so the rank difference is now 2 but
    // we have not yet sent index 0 so it should go out systematically now
    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 0U);
    EXPECT_EQ(stack.systematic_count(), 2U);

    stack.reset();

    // We have sent all packets systematically once - and the rank difference
    // is 2 so we send full
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);

    stack.reset();

    stack.set_systematic_off();
    stack.m_remote_is_symbol_pivot[1] = true;

    // We simulate that the remote device has received packet 1, so now the rank
    // difference is again 1 but we have turned systematic off
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);

    stack.reset();

    stack.set_systematic_on();

    // Rank difference 1 so we send systematically
    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 0U);
    EXPECT_EQ(stack.systematic_count(), 2U);

    stack.reset();

    stack.m_remote_is_symbol_pivot[0] = true;

    // Rank equal so we just code
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);

    stack.reset();

    // Imagine another source sending so the remote rank increase without
    // the local rank increasing.
    stack.m_remote_is_symbol_pivot[5] = true;

    // Rank difference negative we code
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);

    stack.reset();

    stack.m_is_symbol_pivot[2] = true;
    stack.m_is_symbol_pivot[3] = true;

    EXPECT_EQ(stack.rank(), 4U);
    EXPECT_EQ(stack.remote_rank(), 3U);

    // Now two packets are added locally but not the same as added
    // remotely so we have a rank difference of 1 the following will
    // now happen. Since we have not sent the two newly added symbols
    // before we will send them systematically. After doing this the
    // rank difference will still be only 1 but we will switch to full
    // coding because looking at the pivots the rank difference is
    // actually 2
    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 2U);
    EXPECT_EQ(stack.systematic_count(), 3U);

    stack.reset();

    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 3U);
    EXPECT_EQ(stack.systematic_count(), 4U);

    stack.reset();

    // No systematic due to the pivot difference
    stack.encode(&symbol[0], &header[0]);

    EXPECT_TRUE(stack.m_encode_full);
    EXPECT_FALSE(stack.m_encode_systematic);

    stack.m_remote_is_symbol_pivot[3] = true;

    // Lets say the remote now gets packet 3 the rank is now equal
    // but pivot wise the rank difference is 1 so we send symbol 2
    // systematically
    stack.reset();

    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 2U);
    EXPECT_EQ(stack.systematic_count(), 4U);

    stack.m_is_symbol_pivot[5] = true;

    // We now got a packet already at the decoder so we continue trying
    // to send symbol 2
    stack.reset();

    stack.encode(&symbol[0], &header[0]);

    EXPECT_FALSE(stack.m_encode_full);
    EXPECT_TRUE(stack.m_encode_systematic);
    EXPECT_EQ(stack.m_systematic_index, 2U);
    EXPECT_EQ(stack.systematic_count(), 4U);
}


