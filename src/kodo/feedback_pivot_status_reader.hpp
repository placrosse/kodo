// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "feedback_pivot_status.hpp"

namespace kodo
{

    /// @todo document/test
    ///
    /// @ingroup status_api
    ///
    template<class SuperCoder>
    class feedback_pivot_status_reader :
        public feedback_pivot_status<SuperCoder>
    {
    public:

        /// The actual super type
        typedef feedback_pivot_status<SuperCoder> Super;

    public:

        void read_feedback(const uint8_t* feedback)
        {
            assert(feedback);

            Super::read_pivot_status(feedback);
            uint32_t read = Super::pivot_status_size();

            Super::read_feedback(feedback + read);
        }

    };

}
