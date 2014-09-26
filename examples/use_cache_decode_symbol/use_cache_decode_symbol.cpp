// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/cache_decode_symbol.hpp>
#include <kodo/basic_factory.hpp>

namespace kodo
{

    /// @brief The symbol info stack defined below can be used to split an
    ///        incoming encoded payload into the encoded
    ///        symbol data and symbol id (the RLNC encoding vector).
    ///
    ///        You can check the API of the cache_decode_symbol to
    ///        see how to access information about the coded symbol.
    ///
    ///        The info stack is created in such a way that it
    ///        is compatible and can decode symbols produced by a
    ///        full_rlnc_encoder. In case you wish to use a similar approach
    ///        for a different codec stack you should ensure that the
    ///        layers in the two stacks are compatible (i.e. at least "Payload
    ///        API" to "Codec API" are the same).
    template<class Field>
    class symbol_info_decoder : public
        // Payload API
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Decoder API
        cache_decode_symbol<  // <-- Cached symbol decoder
        // Coefficient Storage API
        coefficient_info<
        // Storage API
        storage_bytes_used<
        storage_block_length<
        storage_block_size<
        // Finite Field API
        finite_field_info<Field,
        // Final Layer
        final_layer
        > > > > > > > > > >
    {
    public:
        using factory = basic_factory<symbol_info_decoder>;
    };

}

/// @example use_cache_decode_symbol.cpp
///
/// This example shows how to use the cached symbol decoder to "extract"
/// the symbol coding coefficients and the encoded symbol data from an
/// incoming symbol.
///
/// Note, that if you are just interested in seeing the encoding
/// vector used for the decoding you can also use the trace
/// functionality which makes that type of information available. See
/// the use_trace_layers example for more information.
int main()
{
    // The finite field we will use in the example. You can try
    // with other fields by specifying e.g. fifi::binary8 for the
    // extension field 2^8
    typedef fifi::binary finite_field;

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 8;
    uint32_t symbol_size = 160;

    // Typdefs for the encoder/decoder type we wish to use
    typedef kodo::full_rlnc_encoder<finite_field> rlnc_encoder;
    typedef kodo::full_rlnc_decoder<finite_field> rlnc_decoder;

    typedef kodo::symbol_info_decoder<finite_field> rlnc_info_decoder;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoders.
    // Each stack we use have their own factories.
    rlnc_encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    rlnc_decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    rlnc_info_decoder::factory info_decoder_factory(symbols, symbol_size);
    auto info_decoder = info_decoder_factory.build();

    // Allocate some storage for a "payload" the payload is what we
    // would eventually send over a network
    std::vector<uint8_t> payload(encoder->payload_size());

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    std::vector<uint8_t> data_in(encoder->block_size());

    // Just for fun - fill the data with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Assign the data buffer to the encoder so that we may start to
    // produce encoded symbols from it
    encoder->set_symbols(sak::storage(data_in));

    while (!decoder->is_complete())
    {
        // Encode a packet into the payload buffer
        encoder->encode( payload.data() );

        // Here we "simulate" a packet loss of approximately 50% by
        // dropping half of the encoded packets.  When running this
        // example you will notice that the initial symbols are
        // received systematically (i.e. uncoded). After sending all
        // symbols once uncoded, the encoder will switch to full
        // coding, in which case you will see the full encoding
        // vectors being sent and received.
        if((rand() % 2) == 0)
            continue;

        // Pass the encoded packet to the info decoder. After this
        // information about the coded symbol can be fetched using the
        // cache_decode_symbol API
        info_decoder->decode( payload.data() );

        // The cache should contain data now
        assert(info_decoder->is_cache_valid());

        if (!info_decoder->cached_symbol_coded())
        {
            // The symbol was uncoded so we may ask the cache which of
            // the original symbols we have received.

            std::cout << "Symbol was uncoded, index = "
                      << info_decoder->cached_symbol_index() << std::endl;

            // Now we pass the data directly into our actual
            // decoder. This is done using the "Codec API" directly,
            // and not through the "Payload API" as we would typically
            // do.
            decoder->decode_symbol( info_decoder->cached_symbol_data(),
                                    info_decoder->cached_symbol_index());

        }
        else
        {
            // The symbol was coded so we may ask the cache to return
            // the coding coefficients used to create the encoded
            // symbol.

            std::cout << "Symbol was coded, encoding vector = ";

            const uint8_t* c = info_decoder->cached_symbol_coefficients();

            // We loop through the coefficient buffer and print the
            // coefficients
            for(uint32_t i = 0; i < info_decoder->symbols(); ++i)
            {
                std::cout << (uint32_t) fifi::get_value<finite_field>(c, i)
                          << " ";
            }

            std::cout << std::endl;

            // Pass that packet to the decoder, as with the uncoded
            // symbols above we pass it directly to the "Codec API"
            decoder->decode_symbol(info_decoder->cached_symbol_data(),
                                   info_decoder->cached_symbol_coefficients());

        }
    }

    // The decoder is complete, now copy the symbols from the decoder
    std::vector<uint8_t> data_out(decoder->block_size());
    decoder->copy_symbols(sak::storage(data_out));

    // Check we properly decoded the data
    if (data_out == data_in)
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }
}
