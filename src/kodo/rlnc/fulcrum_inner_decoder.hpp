// Copyright Steinwurf ApS 2011-2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "shallow_full_rlnc_decoder.hpp"
#include "fulcrum_info.hpp"
#include "fulcrum_nested_stack.hpp"
#include "fulcrum_rank_inner_decoder.hpp"
#include "fulcrum_expansion_storage.hpp"

#include "../nested_payload_decoder.hpp"
#include "../nested_catch_all.hpp"

namespace kodo
{
    /// @todo docs
    template<class Field>
    class fulcrum_inner_decoder : public
        // Payload API
        nested_payload_decoder<
        // Codec Header API
        // Symbol ID API
        // Coefficient Generator API
        // Codec API
        fulcrum_rank_inner_decoder<
        // Storage API
        fulcrum_expansion_storage<
        deep_symbol_storage<
        storage_bytes_used<
        storage_block_info<
        // Finite Field API
        finite_field_info<Field,
        // Fulcrum API
        nested_catch_all<
        fulcrum_nested_stack<shallow_full_rlnc_decoder<Field>,
        fulcrum_info<10,4,
        // Factory API
        final_coder_factory_pool<
        // Final type
        fulcrum_inner_decoder<Field>
        > > > > > > > > > > >
    { };
}
