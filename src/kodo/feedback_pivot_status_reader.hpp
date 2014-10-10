// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @ingroup feedback_layers
    ///
    /// @brief This layer reads the pivot status information using the feedback
    ///        convenience API
    template<class SuperCoder>
    class feedback_pivot_status_reader : public SuperCoder
    {
    public:

        /// @copydoc layer::read_feedback(const uint8_t*)
        void read_feedback(const uint8_t* feedback)
        {
            assert(feedback);

            SuperCoder::read_pivot_status(feedback);
            uint32_t read = SuperCoder::pivot_status_size();

            SuperCoder::read_feedback(feedback + read);
        }
    };
}
