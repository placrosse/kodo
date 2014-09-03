// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "mapped_file_source.hpp"
#include "object_filename.hpp"
#include "stack_factory.hpp"
#include "rfc5052_partitioning.hpp"
#include "object_storage.hpp"

#include "../rebind_factory.hpp"
#include "../final_layer.hpp"

namespace kodo
{
namespace object
{
    /// @todo finish docs
    template<class Stack>
    class file_encoder : public
        mapped_file_source<
        object_filename<
        object_storage<
        stack_factory<Stack,
        rfc5052_partitioning<
        final_layer> > > > >
    {
    public:

        /// We will use the same factory as the stack
        using factory = rebind_factory<Stack, file_encoder>;
    };
}
}
