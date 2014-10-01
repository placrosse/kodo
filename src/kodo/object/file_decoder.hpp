// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "mapped_file_sink.hpp"
#include "object_filename.hpp"
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
    /// @brief A file decoder creates a number of decoders over a
    ///        file in the filesystem
    ///
    /// If the file is too large for a single encoder/decoder pair one
    /// the file decoder will make sure that we create as many
    /// encoder/decoder pairs as we need.
    ///
    /// For an example of how it works see the encode_decode_file
    /// example in the /kodo/examples folder.
    ///
    template
    <
        class Stack,
        class BlockPartitioning = rfc5052_partitioning_scheme
    >
    class file_decoder : public
        mapped_file_sink<
        object_filename<
        is_complete_decoder<
        object_storage<
        stack_factory<
            wrap_is_complete_callback_decoder<
            wrap_restore_partial_symbol_decoder<Stack>>,
        partitioning<BlockPartitioning,
        final_layer>>>>>>
    {
    public:

        /// We will use the same factory as the stack
        using factory = rebind_factory<Stack, file_decoder>;

        /// This decoder only works with a shallow stack. The reason
        /// for this is that we are using the mutable_object_storage
        /// layer which will call the set_symbols() function on each
        /// decoder built. So we want the decoder to be shallow and
        /// therefore use that memory to store the decoded data.
        ///
        static_assert(
            has_mutable_shallow_symbol_storage<Stack>::value,
            "The file decoder only works with decoders using"
            "shallow storage");
    };
}
}
