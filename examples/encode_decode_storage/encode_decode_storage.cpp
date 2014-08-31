// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstring>
#include <utility>
#include <kodo/object/shallow_storage_decoder.hpp>
#include <kodo/storage_encoder.hpp>
#include <kodo/rlnc/full_rlnc_codes.hpp>

/// @todo re-enable this example

/// @example encode_decode_storage.cpp
///
/// Often we want to encode / decode data that exceed a single
/// encoding/decoding block. In this case we need to "chop" up
/// the data into manageable chunks and then encode and decode
/// each chuck separately. This examples shows how to use the
/// storage encoder and decoder in Kodo.
///
/// Both the encoder and decoder uses a shallow storage which means
/// that they operate directly on the memory provided.

// @todo Proposal is to put the layers related to object encoding/decoding inot
//       a object namespace. This means we will have things like:
//
//       + kodo::object::shallow_storage
//       + kodo::object::stack_factory
//       + kodo::object::rfc5052_partitioning
//       + kodo::object::is_complete
//
//       This should be pretty straight forward the only issue I can
//       see right now is that we might have a class or helper
//       function in another namesapce e.g. kodo::file or in kodo
//       itself which has the same name. In it self this does not
//       cause any problems, but our test naming convention would have
//       to be updated.
//
//       One way to deal with this is to also put things in the object
//       namespace into a sub-folder in kodo. So the class
//       kodo::object::shallow_storage would be located in the header:
//
//       src/kodo/object/shallow_storage.hpp
//
//       And tested in test/src/object/test_shallow_storage.cpp
//
//       This keeps the convention of having a class named xyz tested
//       in test_xyz.cpp it only affect which folder the test is in.
//
//       Defining multiple namespaces leads to quite a bit of {
//       bracket indentation and nesting. I therefore would propose to
//       do it the same way boost does it:
//
//       namespace kodo
//       {
//       namespace object
//       {
//
//           template<class SuperCoder>
//           class shallow_storage : public SuperCoder
//           {
//           ....
//           };
//
//       }
//       }
//
//
// kodo::object::encoder
// kodo::file::encoder
// kodo::random_annex::encoder
//


int main()
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 64;

    uint32_t object_size = 23456;

    using storage_encoder = kodo::storage_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary> >;

    using storage_decoder = kodo::object::shallow_storage_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary> >;

    storage_encoder::factory encoder_factory(max_symbols, max_symbol_size);
    storage_decoder::factory decoder_factory(max_symbols, max_symbol_size);

    std::vector<uint8_t> data_out(object_size, '\0');
    std::vector<uint8_t> data_in(object_size, 'x');

    encoder_factory.set_storage(sak::storage(data_in));
    decoder_factory.set_storage(sak::storage(data_out));

    auto object_encoder = encoder_factory.build();
    auto object_decoder = decoder_factory.build();

    std::cout << "object_size = " << object_size << std::endl;
    std::cout << "encoder blocks = " << object_encoder->blocks() << std::endl;
    std::cout << "decoder blocks = " << object_decoder->blocks() << std::endl;

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

    // Check we properly decoded the data
    if (data_in == data_out)
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }
}
