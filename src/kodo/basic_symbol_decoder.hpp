// Copyright Steinwurf ApS 2011-2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "common_decoder_layers.hpp"
#include "coefficient_storage_layers.hpp"
#include "deep_storage_layers.hpp"
#include "finite_field_layers.hpp"
#include "pool_factory.hpp"

namespace kodo
{
    /// @todo docs + tests
    ///
    /// @brief The basic symbol decoder implements the raw symbol
    ///        decoding operations.
    ///
    /// By raw decoding operations we mean the:
    /// - layer::decode_symbol(uint8_t*,uint8_t*)
    /// - layer::decode_symbol(uint8_t*,uint32_t)
    ///
    /// I.e non of the payload layers are included in this codec stack.
    template<class Field, class TraceTag = kodo::disable_trace>
    class basic_symbol_decoder : public
        // Decoder API
        common_decoder_layers<TraceTag,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer> > > >
    {
    public:
        using factory = pool_factory<basic_symbol_decoder>;
    };
}
