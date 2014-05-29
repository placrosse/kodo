// Copyright Steinwurf Apes 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup codec_header_layers
    ///
    /// @brief Allows the user to turn on/off the systematic phase
    ///
    /// This layer is used when implementing encoders that support
    /// systematic operations. The systematic encoder will produce
    /// systematic packets only when in the "systematic phase" this
    /// layer makes it possible to enable or disable the systematic
    /// phase. By setting the template argument SystematicOn on can
    /// control what should be the default.
    template<bool SystematicOn, class SuperCoder>
    class default_systematic_phase : public SuperCoder
    {
    public:

        /// Constructor
        default_systematic_phase()
            : m_systematic(SystematicOn)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            /// Reset the state
            m_systematic = SystematicOn;
        }

        /// @return true if we are in the systematic phase (i.e. there
        /// are systematic packet to send) otherwise false
        bool in_systematic_phase() const
        {
            if(m_systematic)
            {
                return SuperCoder::in_systematic_phase();
            }
            else
            {
                return false;
            }
        }

        /// @return, true if the encoder is in systematic mode
        bool is_systematic_on() const
        {
            return m_systematic;
        }

        /// Set the encoder in systematic mode
        void set_systematic_on()
        {
            m_systematic = true;
        }

        /// Turns off systematic mode
        void set_systematic_off()
        {
            m_systematic = false;
        }

    protected:

        /// Allows the systematic mode to be disabled at run-time
        bool m_systematic;

    };

}

