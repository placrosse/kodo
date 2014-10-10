// Copyright Steinwurf ApS 2011-2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../payload_decoder.hpp"
#include "../systematic_decoder.hpp"
#include "../symbol_id_decoder.hpp"
#include "../plain_symbol_id_reader.hpp"
#include "../coefficient_info.hpp"
#include "../storage_block_info.hpp"
#include "../finite_field_info.hpp"
#include "../proxy_layer.hpp"
#include "../final_layer.hpp"
#include "../pool_factory.hpp"

namespace kodo
{
    /// @todo test + docs
    /// Decodes the payload portion of the inner code in a fulcrum decoder
    template<class MainStack>
    class fulcrum_payload_decoder : public
        // Payload API
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Coefficient Storage API
        coefficient_info<
        // Storage API
        storage_block_info<
        // Finite Field API
        finite_field_info<fifi::binary,
        // Proxy
        proxy_layer<MainStack,
        // Final layer
        final_layer
        >>>>>>>>
    {
    public:
        using factory = pool_factory<fulcrum_payload_decoder>;
    };
}
