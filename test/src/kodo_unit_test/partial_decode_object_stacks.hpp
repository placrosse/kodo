// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

namespace kodo
{
    /// Test helper for running though an object encoder/decoder.
    ///
    /// It will run though the blocks of the object in linear order,
    /// but in the first run it will not fully complete decoding. In
    /// the second iteration it will start over.
    ///
    template<class SuperTest>
    class partial_decode_object_stacks : public SuperTest
    {
    public:

        partial_decode_object_stacks(uint32_t max_symbols,
                                     uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size)
        { }

        void run()
        {
            auto file_encoder = SuperTest::encoder_factory().build();
            auto file_decoder = SuperTest::decoder_factory().build();

            EXPECT_TRUE(file_encoder->blocks() > 0);
            EXPECT_EQ(file_encoder->blocks(), file_decoder->blocks());

            // In the first loop we decode until we just need one more
            // symbol to decode then we move on.
            for (uint32_t i = 0; i < file_encoder->blocks(); ++i)
            {
                auto e = file_encoder->build(i);
                auto d = file_decoder->build(i);

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

            // Run the encoding/decoding loop again, by this we will
            // build the same encoders and decoders as in the previous
            // loop
            for (uint32_t i = 0; i < file_encoder->blocks(); ++i)
            {
                auto e = file_encoder->build(i);
                auto d = file_decoder->build(i);

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
