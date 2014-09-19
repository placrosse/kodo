// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

namespace kodo
{
    /// Test helper for running though an object encoder/decoder.
    ///
    /// It will run though the blocks of the object in linear order,
    /// but in the first run it will not fully complete decoding. In
    /// the second iteration it will rebuild the object stacks and
    /// start over.
    ///
    template<class SuperTest>
    class rebuild_object_stacks : public SuperTest
    {
    public:

        rebuild_object_stacks(uint32_t max_symbols,
                                     uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            auto object_encoder = SuperTest::encoder_factory().build();
            auto object_decoder = SuperTest::decoder_factory().build();

            EXPECT_TRUE(object_encoder->blocks() > 0);
            EXPECT_EQ(object_encoder->blocks(), object_decoder->blocks());

            // In the first loop we decode until we just need one more
            // symbol to decode then we move on.
            for (uint32_t i = 0; i < object_encoder->blocks(); ++i)
            {
                auto e = object_encoder->build(i);
                auto d = object_decoder->build(i);

                std::vector<uint8_t> payload(e->payload_size());

                while (d->rank() < d->symbols() - 1)
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

            // Release the current object encoder/decoder
            object_encoder.reset();
            object_decoder.reset();

            object_encoder = SuperTest::encoder_factory().build();
            object_decoder = SuperTest::decoder_factory().build();

            // Run the encoding/decoding loop again, by this we will
            // build the same encoders and decoders as in the previous
            // loop
            for (uint32_t i = 0; i < object_encoder->blocks(); ++i)
            {
                auto e = object_encoder->build(i);
                auto d = object_decoder->build(i);

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
