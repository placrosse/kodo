// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @example rank_callback.cpp
///
/// It may be that we want a function to be called on some event
/// within the decoder.  This can be done using callback functions.
/// The following example illustrates how this can be done by adding
/// the rank_callback_decoder layer to the decoder stack and how the
/// rank changed event can be handled in three different ways. Other
/// callback layers could also be used instead of the rank callback
/// layer provided that they are added at the correct position in the
/// stack.


#include <kodo/rlnc/full_rlnc_codes.hpp>
#include <kodo/rank_callback_decoder.hpp>

#include <functional>
#include <vector>

namespace kodo
{
    // Added rank_callback layer to decoder stack
    template<class Field>
    class full_rlnc_callback_decoder : public
        // Payload API
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Codec API

        // rank_callback layer is inserted in "Codec API" which it
        // designed for. It has to be inserted into the stack above
        // layers that can change the rank during decoding
        rank_callback_decoder<

        aligned_coefficients_decoder<
        forward_linear_block_decoder<
        symbol_decoding_status_counter<
        symbol_decoding_status_tracker<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_storage<
        coefficient_info<
        // Storage API
        deep_symbol_storage<
        storage_bytes_used<
        storage_block_length<
        storage_block_size<
        // Finite Field API
        finite_field_math<typename fifi::default_field<Field>::type,
        finite_field_info<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > > > > > > >
    {
    public:
        using factory = basic_factory<full_rlnc_callback_decoder>;
    };
}

// Typdefs for the encoder/decoder type we wish to use
using rlnc_encoder = kodo::full_rlnc_encoder<fifi::binary8>;
using rlnc_decoder = kodo::full_rlnc_callback_decoder<fifi::binary8>;

// Global function as callback handler
void rank_changed_event(uint32_t rank)
{
    std::cout << "Rank changed to " << rank << std::endl;
}

int main()
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 8;
    uint32_t symbol_size = 160;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoders
    rlnc_encoder::factory encoder_factory(symbols, symbol_size);
    auto encoder = encoder_factory.build();

    rlnc_decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    // Set callback handler
    decoder->set_rank_changed_callback(
        std::bind(rank_changed_event, std::placeholders::_1));

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    std::vector<uint8_t> payload(encoder->payload_size());

    // Allocate some data to encode. In this case we make a buffer with the
    // same size as the encoder's block size (the max. amount a single encoder
    // can encode)
    std::vector<uint8_t> data_in(encoder->block_size(), 'x');


    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    encoder->set_symbols(sak::storage(data_in));

    while (!decoder->is_complete())
    {
        // Encode a packet into the payload buffer
        encoder->encode(payload.data());

        // Pass that packet to the decoder
        decoder->decode(payload.data());
    }

    // The decoder is complete, now copy the symbols from the decoder
    std::vector<uint8_t> data_out(decoder->block_size());
    decoder->copy_symbols(sak::storage(data_out));

    // Check we properly decoded the data
    if (std::equal(data_out.begin(), data_out.end(), data_in.begin()))
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }
}
