// Copyright Steinwurf ApS 2011-2013
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once


#include "../nested_payload_decoder.hpp"
#include "../shallow_symbol_storage.hpp"
#include "../rlnc/full_vector_codes.hpp"
#include "../catch_all_nested_stack.hpp"
#include "fulcrum_info.hpp"
#include "fulcrum_nested_stack.hpp"
#include "fulcrum_expansion_storage.hpp"
#include "fulcrum_storage_inner_decoder.hpp"

namespace kodo
{

    template<class Field>
    class shallow_full_rlnc_decoder :
        public // Payload API
               payload_recoder<recoding_stack,
               payload_decoder<
               // Codec Header API
               systematic_decoder<
               symbol_id_decoder<
               // Symbol ID API
               plain_symbol_id_reader<
               // Decoder API
               aligned_coefficients_decoder<
               forward_linear_block_decoder<
               symbol_decoding_status_counter<
               symbol_decoding_status_tracker<
               // Coefficient Storage API
               coefficient_value_access<
               coefficient_storage<
               coefficient_info<
               // Storage API
               mutable_shallow_symbol_storage<
               storage_bytes_used<
               storage_block_info<
               // Finite Field API
               finite_field_math<typename fifi::default_field<Field>::type,
               finite_field_info<Field,
               // Factory API
               final_coder_factory_pool<
               // Final type
               shallow_full_rlnc_decoder<Field>
               > > > > > > > > > > > > > > > > > >
    { };

    template<class SuperCoder>
    class fulcrum_info_inner_decoder : public SuperCoder
    {
    public:

        uint32_t rank() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);

            uint32_t rank = nested->rank();
            uint32_t expansion = SuperCoder::expansion();

            if(rank < expansion)
            {
                return 0;
            }
            else
            {
                return rank - expansion;
            }

        }
    };

    template<class Field>
    class fulcrum_inner_decoder :
        public // Payload API
               nested_payload_decoder<
               // Codec Header API
               // Symbol ID API
               // Coefficient Generator API
               // Codec API
               fulcrum_info_inner_decoder<
               // Storage API
               fulcrum_expansion_storage<
               deep_symbol_storage<
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
               fulcrum_inner_decoder<Field>
                   > > > > > > > > > > >
    { };

    template<class Field>
    class shallow_fulcrum_inner_decoder :
        public // Payload API
               nested_payload_decoder<
               // Codec Header API
               // Symbol ID API
               // Coefficient Generator API
               // Codec API
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
               > > > > > > > > > >
    { };


}





