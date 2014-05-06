// Copyright Steinwurf ApS 2011-2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../mutable_shallow_symbol_storage.hpp"

#include "fulcrum_inner_decoder.hpp"

namespace kodo
{
    /// @todo docs + tests
    template<class Field>
    class shallow_fulcrum_inner_decoder : public
        // Payload API
        nested_payload_decoder<
        // Codec Header API
        // Symbol ID API
        // Coefficient Generator API
        // Codec API
        fulcrum_rank_inner_decoder<
        // Storage API
        fulcrum_expansion_storage<
        mutable_shallow_symbol_storage<
        storage_bytes_used<
        storage_block_info<
        // Finite Field API
        finite_field_info<Field,
        // Fulcrum API
        catch_all_nested_stack<
        fulcrum_nested_stack<shallow_full_rlnc_decoder<Field>,
        fulcrum_info<10,4,
        // Factory API
        final_coder_factory_pool<
        // Final type
        shallow_fulcrum_inner_decoder<Field>
        > > > > > > > > > > >
    { };
}
