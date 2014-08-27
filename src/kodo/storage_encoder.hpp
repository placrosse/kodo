// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "object_stack_builder.hpp"
#include "object_stack.hpp"
#include "rfc5052_object_partitioning.hpp"
#include "const_object_storage.hpp"
#include "final_layer.hpp"

namespace kodo
{
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
    /// Example:
    ///
    /// @todo finish docs
    template<class Stack>
    class storage_encoder : public
        object_storage<
        object_stack_builder<Stack,
//        object_stack<Stack,
        rfc5052_object_partitioning<
            final_layer> > > //>
    {
    public:

        /// The factory we will use for the object encoder
        using factory = basic_factory<storage_encoder>;
    };
}
