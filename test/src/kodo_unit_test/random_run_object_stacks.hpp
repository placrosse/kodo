// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <chrono>
#include <numeric>
#include <random>

namespace kodo
{
    /// Test helper for running though an object encoder/decoder.
    ///
    /// It will run though the blocks of the object in random order
    ///
    template<class SuperTest>
    class random_run_object_stacks : public SuperTest
    {
    public:

        random_run_object_stacks(uint32_t max_symbols,
                                 uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            auto object_encoder = SuperTest::encoder_factory().build();
            auto object_decoder = SuperTest::decoder_factory().build();

            EXPECT_TRUE((bool) object_encoder);
            EXPECT_TRUE((bool) object_decoder);

            EXPECT_TRUE(object_encoder->blocks() > 0);
            EXPECT_TRUE(object_decoder->blocks() > 0);

            EXPECT_EQ(object_encoder->blocks(), object_decoder->blocks());

            // Generate the sequence of block ids which we will go through
            std::vector<uint32_t> blocks(object_encoder->blocks());
            std::iota(blocks.begin(), blocks.end(), 0U);

            // Shuffle the block ids
            auto time = std::chrono::system_clock::now();
            auto seed = time.time_since_epoch().count();

            std::shuffle(blocks.begin(), blocks.end(),
                         std::default_random_engine(seed));

            for (uint32_t i = 0; i < blocks.size(); ++i)
            {
                auto id = blocks[i];

                auto e = object_encoder->build(id);
                auto d = object_decoder->build(id);

                EXPECT_TRUE((bool) e);
                EXPECT_TRUE((bool) d);

                EXPECT_TRUE(e->payload_size() > 0);
                EXPECT_TRUE(d->payload_size() > 0);

                EXPECT_EQ(e->payload_size(), d->payload_size());

                std::vector<uint8_t> payload(e->payload_size());

                while (!d->is_complete())
                {
                    e->encode( payload.data() );

                    // Here we would send and receive the payload over a
                    // network. Lets throw away some packet to simulate.
                    if (rand() % 2)
                    {
                        continue;
                    }

                    d->decode( payload.data() );
                }
            }
        }
    };
}
