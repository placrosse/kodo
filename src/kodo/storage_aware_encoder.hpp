// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup encoder_layers
    /// @brief Implements the layer::rank(), layer::is_symbol_pivot() and
    ///        layer::is_symbol_uncoded() functions based on the storage
    ///        status.
    ///
    /// This kind of functionality is needed when an encoder should be
    /// able to support encoding before all symbols are ready. The
    /// coefficient generator layers uses this information to create
    /// partial coefficient vectors if needed.  Furthermore when
    /// working with systematic decision layers it sometimes can be
    /// necessary to know whether a symbol is uncoded. A concrete
    /// example is the remote_pivot_aware_systematic_phase layer which
    /// will use pivot information from an upstream decoder to
    /// determine whether to switch to systematic coding (when the
    /// decoder is only missing one symbol currently available as
    /// uncoded)
    template<class SuperCoder>
    class storage_aware_encoder : public SuperCoder
    {
    public:

        /// @copydoc layer::rank() const
        uint32_t rank() const
        {
            return SuperCoder::symbols_initialized();
        }

        /// @copydoc layer::is_symbol_pivot(uint32_t) const
        bool is_symbol_pivot(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());
            return SuperCoder::is_symbol_initialized(index);
        }

        /// @copydoc layer::symbols_uncoded() const
        uint32_t symbols_uncoded() const
        {
            return SuperCoder::symbols_initialized();
        }

        /// @copydoc layer::is_symbol_uncoded(uint32_t) const
        bool is_symbol_uncoded(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());
            return SuperCoder::is_symbol_initialized(index);
        }

    };

}


