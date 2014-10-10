// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "is_complete_decoder.hpp"
#include "stack_factory.hpp"
#include "partitioning.hpp"
#include "object_storage.hpp"

#include "../rebind_factory.hpp"
#include "../final_layer.hpp"
#include "../wrap_restore_partial_symbol_decoder.hpp"
#include "../wrap_is_complete_callback_decoder.hpp"
#include "../has_mutable_shallow_symbol_storage.hpp"
#include "../rfc5052_partitioning_scheme.hpp"

namespace kodo
{
namespace object
{
    /// @ingroup object_fec_stacks
    ///
    /// @brief A storage decoder creates a number of decoders over a
    ///        sak::const_storage object.
    ///
    /// The purpose of the storage decoder is to provide functionality
    /// to decode objects larger than a single block/generation. The
    /// storage decoder will use a partitioning scheme to create
    /// multiple decoders each covering a different part of the object
    /// being decoded.
    ///
    /// For an example of how it works see the encode_decode_storage
    /// example in the /kodo/examples folder.
    ///
    template
    <
        class Stack,
        class BlockPartitioning = rfc5052_partitioning_scheme
    >
    class storage_decoder : public
        is_complete_decoder<
        object_storage<
        stack_factory<
            wrap_is_complete_callback_decoder<
            wrap_restore_partial_symbol_decoder<Stack>>,
        partitioning<BlockPartitioning,
        final_layer>>>>
    {
    public:

        /// We will use the same factory as the stack
        using factory = rebind_factory<Stack, storage_decoder>;

        /// This decoder only works with a shallow stack. The reason
        /// for this is that we are using the mutable_object_storage
        /// layer which will call the set_symbols() function on each
        /// decoder built. So we want the decoder to be shallow and
        /// therefore use that memory to store the decoded data.
        ///
        static_assert(
            has_mutable_shallow_symbol_storage<Stack>::value,
            "The storage decoder only works with decoders using"
            "shallow storage");
    };
}
}
