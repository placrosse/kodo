// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

namespace kodo
{
    /// Define an encoder and decoder factory for building the stacks
    /// to be tested.
    template<class Encoder, class Decoder, class SuperTest>
    class declare_stack_factories : public SuperTest
    {
    public:

        using encoder_type = Encoder;
        using decoder_type = Decoder;
        using encoder_factory_type = typename encoder_type::factory;
        using decoder_factory_type = typename decoder_type::factory;

    public:

        declare_stack_factories(uint32_t max_symbols, uint32_t max_symbol_size)
            : SuperTest(max_symbols, max_symbol_size),
              m_encoder_factory(max_symbols, max_symbol_size),
              m_decoder_factory(max_symbols, max_symbol_size)
        { }

        encoder_factory_type& encoder_factory()
        {
            return m_encoder_factory;
        }

        decoder_factory_type& decoder_factory()
        {
            return m_decoder_factory;
        }

    private:

        encoder_factory_type m_encoder_factory;
        decoder_factory_type m_decoder_factory;
    };
}
