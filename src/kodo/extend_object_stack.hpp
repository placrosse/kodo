// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <sak/storage.hpp>

#include "rfc5052_partitioning_scheme.hpp"
#include "final_layer.hpp"
#include "basic_factory.hpp"
#include "storage_block_size.hpp"

namespace kodo
{
    /// @todo docs
    template<template <class> class StackWrapper, class SuperCoder>
    class extend_object_stack : public SuperCoder
    {
    public:
        using stack_type = StackWrapper<typename SuperCoder::stack_type>;
    };
}
