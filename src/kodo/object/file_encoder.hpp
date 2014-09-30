// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "mapped_file_source.hpp"
#include "object_filename.hpp"
#include "stack_factory.hpp"
#include "partitioning.hpp"
#include "object_storage.hpp"

#include "../rebind_factory.hpp"
#include "../final_layer.hpp"
#include "../rfc5052_partitioning_scheme.hpp"

namespace kodo
{
namespace object
{
    /// @ingroup object_fec_stacks
    ///
    /// @brief A file encoder creates a number of encoders over a
    ///        file in the filesystem
    ///
    /// If the file is too large for a single encoder/decoder pair one
    /// the file encoder will make sure that we create as many
    /// encoder/decoder pairs as we need.
    ///
    /// For an example of how it works see the encode_decode_file
    /// example in the /kodo/examples folder.
    ///
    template
    <
        class Stack,
        class PartitioningScheme = rfc5052_partitioning_scheme
    >
    class file_encoder : public
        mapped_file_source<
        object_filename<
        object_storage<
        stack_factory<Stack,
        partitioning<PartitioningScheme,
        final_layer>>>>>
    {
    public:

        /// We will use the same factory as the stack
        using factory = rebind_factory<Stack, file_encoder>;
    };
}
}
