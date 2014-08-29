// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "object_stack_builder.hpp"
#include "object_stack.hpp"
#include "rfc5052_object_partitioning.hpp"
#include "mutable_object_storage.hpp"
#include "final_layer.hpp"

#include "extend_object_stack.hpp"
#include "wrap_restore_partial_symbol_decoder.hpp"

namespace kodo
{
    /// @todo add docs
    ///
    /// The shallow variant of the storage decoder refers to the fact
    /// that it will decode the data in a user provided buffer. So the
    /// decoder does not maintain any internal data buffer for the
    /// decoded data. Since this is the case the user must provide the
    /// memory where the decoded object should be place when building
    /// the object decoder.
    ///
    /// Example:
    ///
    ///
    ///
    template<class Stack>
    class shallow_storage_decoder : public
        object_storage<
        object_stack_builder<
            wrap_restore_partial_symbol_decoder<Stack>,
        // extend_object_stack<wrap_restore_partial_symbol_decoder,
            // object_stack<wrap_restore_partial_symbol_decoder<Stack>,
        rfc5052_object_partitioning<
            final_layer> > > //> //>
    {
    public:

        /// The factory we will use for the object decoder
        using factory = basic_factory<shallow_storage_decoder>;

        /// This decoder only works with a shallow stack. The reason
        /// for this is that we are using the mutable_object_storage
        /// layer which will call the set_symbols() function on each
        /// decoder built. So we want the decoder to be shallow and
        /// therefore use that memory to store the decoded data.
        ///
        /// @todo add static assert
    };
}
