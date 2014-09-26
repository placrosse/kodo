// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

namespace kodo
{
    /// Test helper for running though an object encoder/decoder.
    ///
    /// It will run though the blocks of the object in linear order
    ///
    template<class SuperTest>
    class basic_run_object_stacks : public SuperTest
    {
    public:

        basic_run_object_stacks(uint32_t max_symbols, uint32_t max_symbol_size)
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

            for (uint32_t i = 0; i < object_encoder->blocks(); ++i)
            {
                auto e = object_encoder->build(i);
                auto d = object_decoder->build(i);

                EXPECT_TRUE((bool) e);
                EXPECT_TRUE((bool) d);

                EXPECT_TRUE(e->payload_size() > 0);
                EXPECT_TRUE(d->payload_size() > 0);

                EXPECT_EQ(e->payload_size(), d->payload_size());

                std::vector<uint8_t> payload(e->payload_size());

                EXPECT_FALSE(d->is_complete());

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
