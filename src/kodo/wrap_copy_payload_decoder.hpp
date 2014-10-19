// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "copy_payload_decoder.hpp"
#include "rebind_factory.hpp"

namespace kodo
{
    /// @ingroup payload_codec_layers
    ///
    /// @brief This helper is used to wrap an exting codec stack with
    ///        the copy_payload_decoder layer. See the copy_payload_decoder
    ///        class for more information.
    template<class Codec>
    class wrap_copy_payload_decoder : public copy_payload_decoder<Codec>
    {
    public:

        /// Rebind the factory to build the correct stack
        using factory = rebind_factory<Codec, wrap_copy_payload_decoder>;
    };
}
