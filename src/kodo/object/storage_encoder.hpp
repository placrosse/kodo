// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "stack_factory.hpp"
#include "rfc5052_partitioning.hpp"
#include "object_storage.hpp"

#include "../rebind_factory.hpp"
#include "../final_layer.hpp"

namespace kodo
{
namespace object
{
    /// @ingroup object_fec_stacks
    ///
    /// @brief A storage encoder creates a number of encoders over a
    ///        sak::const_storage object.
    ///
    /// If the sak::const_storage object is too large for a single
    /// encoder/decoder pair one may use the storage encoder.
    ///
    /// One thing to notice is that we do not need both a shallow and
    /// deep variant of the storage encoder. The reason for this is
    /// that for encoding the user always have to set the data to be
    /// encoded using the set_storage function. So this will correctly
    /// initialize both a shallow and deep encoder. However, be aware
    /// that if using a shallow encoder it is the responsability of
    /// the user to keep the data alive for as long as the object
    /// encoder is in use.
    ///
    /// For an example of how it works see the encode_decode_storage
    /// example in the /kodo/examples folder.
    ///
    template<class Stack>
    class storage_encoder : public
        object_storage<
        stack_factory<Stack,
        rfc5052_partitioning<
        final_layer> > >
    {
    public:

        /// We will use the same factory as the stack
        using factory = rebind_factory<Stack, storage_encoder>;
    };
}
}
