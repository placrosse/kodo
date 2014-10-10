// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "feedback_pivot_status.hpp"

namespace kodo
{

    /// @ingroup feedback_layers
    ///
    /// @brief This layer writes the pivot status information using
    ///        the feedback convenience API
    template<class SuperCoder>
    class feedback_pivot_status_writer : public SuperCoder
    {
    public:

        /// @copydoc layer::write_feedback(uint8_t*) const
        uint32_t write_feedback(uint8_t* feedback) const
        {
            assert(feedback);

            SuperCoder::write_pivot_status(feedback);
            uint32_t written = SuperCoder::pivot_status_size();

            return SuperCoder::write_feedback(feedback + written) + written;
        }

    };

}
