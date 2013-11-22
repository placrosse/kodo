// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "final_feedback_size.hpp"

namespace kodo
{

    /// @todo document/test
    ///
    /// @ingroup status_api
    ///
    template<class SuperCoder>
    class final_feedback_writer : public final_feedback_size<SuperCoder>
    {
    public:

        uint32_t write_feedback(uint8_t* feedback) const
        {
            assert(feedback);
            return 0;
        }

    };

}
