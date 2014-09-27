// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>

#include "has_decode_symbol.hpp"

namespace kodo
{
    /// @ingroup generic_api
    /// @ingroup decoder_layers
    ///
    /// This function calls the layer::decode_symbol(std::ostream&) function
    /// used to produce a decode_symbol trace for the chosen codec.
    template
    <
        class T,
        typename std::enable_if<has_decode_symbol<T>::value, uint8_t>::type = 0
    >
    inline void decode_symbol(T& t, uint8_t *symbol_data,
                              uint8_t *symbol_coefficients)
    {
        t.decode_symbol(symbol_data, symbol_coefficients);
    }

    /// @ingroup generic_api
    /// @ingroup decoder_layers
    /// @copydoc decode_symbol(const T&, std::ostream)
    template
    <
        class T,
        typename std::enable_if<!has_decode_symbol<T>::value, uint8_t>::type = 0
    >
    inline void decode_symbol(T& t, uint8_t *symbol_data,
                              uint8_t *symbol_coefficients)
    {
        (void) t;
        (void) symbol_data;
        (void) symbol_coefficients;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // decode_symbol() function. However, this assert can
        // be avoided by using the has_decode_symbol
        assert(0);
    }

    /// @ingroup generic_api
    /// @ingroup decoder_layers
    ///
    /// This function calls the layer::decode_symbol(std::ostream&) function
    /// used to produce a decode_symbol trace for the chosen codec.
    template
    <
        class T,
        typename std::enable_if<has_decode_symbol<T>::value, uint8_t>::type = 0
    >
    inline void decode_symbol(T& t, uint8_t *symbol_data, uint32_t symbol_index)
    {
        t.decode_symbol(symbol_data, symbol_index);
    }

    /// @ingroup generic_api
    /// @ingroup decoder_layers
    /// @copydoc decode_symbol(const T&, std::ostream)
    template
    <
        class T,
        typename std::enable_if<!has_decode_symbol<T>::value, uint8_t>::type = 0
    >
    inline void decode_symbol(T& t, uint8_t *symbol_data, uint32_t symbol_index)
    {
        (void) t;
        (void) symbol_data;
        (void) symbol_index;

        // We do the assert here - to make sure that this call is not
        // silently ignored in cases where the stack does not have the
        // decode_symbol() function. However, this assert can
        // be avoided by using the has_decode_symbol
        assert(0);
    }


}
