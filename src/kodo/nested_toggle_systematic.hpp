// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

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
            auto& nested = SuperCoder::nested();
            assert(nested);
            nested->set_systematic_on();

        }

        /// Turns off systematic mode
        void set_systematic_off()
        {
            auto& nested = SuperCoder::nested();
            assert(nested);
            nested->set_systematic_off();

            /// @todo consider also calling
            ///       Super::set_systeamtic_off() potentially using a
            ///       type trait to only do it if the lower layers
            ///       also have a set_systematic_off .. the same
            ///       counts for set_systematic_on
        }

    };
}
