// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "set_systematic_on.hpp"
#include "set_systematic_off.hpp"

namespace kodo
{
    /// @ingroup codec_header_layers
    ///
    /// @brief Redirects calls to toggle the systematic coding on or
    ///        off to the nested stack.
    template<class SuperCoder>
    class nested_toggle_systematic : public SuperCoder
    {
    public:

        /// Set the encoder in systematic mode
        void set_systematic_on()
        {
            auto&& nested = SuperCoder::nested();
            assert(nested);
            nested->set_systematic_on();

            if (has_set_systematic_on<SuperCoder>::value)
            {
                kodo::set_systematic_on<SuperCoder>(*this);
            }
        }

        /// Turns off systematic mode
        void set_systematic_off()
        {
            auto&& nested = SuperCoder::nested();
            assert(nested);
            nested->set_systematic_off();

            if (has_set_systematic_off<SuperCoder>::value)
            {
                kodo::set_systematic_off<SuperCoder>(*this);
            }
        }
    };
}
