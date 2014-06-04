// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "nested_payload_size.hpp"

namespace kodo
{

    /// @todo test + review + ingroup
    template<class SuperCoder>
    class nested_payload_encoder : public nested_payload_size<SuperCoder>
    {
    public:

        /// @copydoc layer::encode(uint8_t*)
        uint32_t encode(uint8_t *payload)
        {
            assert(payload);
            return SuperCoder::nested()->encode(payload);
        }

    };

}
