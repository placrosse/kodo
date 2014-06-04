// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "nested_payload_size.hpp"

namespace kodo
{
    /// @todo test + docs
    ///
    /// The nested payload decoder layer redirects all calls to
    /// the layer::decode(uint8_t*) function to a nested stack.
    /// In addition to forwarding the layer::decode(uint8_t*)
    /// function the layer also forwards the
    /// layer::factory::max_payload_size() const and layer::payload_size() const
    /// functions to ensure that buffers allocated by the user has the proper
    /// sizes.
    ///
    /// On common use-case for this type of layer are situations where a
    /// nested stack should e.g. "preprocess" the incoming symbols.
    template<class SuperCoder>
    class nested_payload_decoder : public nested_payload_size<SuperCoder>
    {
    public:

        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t* payload)
        {
            assert(payload);
            assert(SuperCoder::nested());
            SuperCoder::nested()->decode(payload);
        }
    };
}
