// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>

#include "disable_trace.hpp"
#include "enable_trace.hpp"
#include "trace.hpp"
#include "hexdump.hpp"

namespace kodo
{
    /// Fall-through case for the case where TraceTag is disable_trace
    template<class TraceTag, class SuperCoder>
    class trace_symbol_storage : public SuperCoder
    {
        static_assert(std::is_same<TraceTag, disable_trace>::value,
                      "Unexpected TraceTag should be disable_trace in the "
                      "fall-through case.");
    };

    /// @ingroup trace
    /// @ingroup symbol_storage_layers
    ///
    /// @brief Print functions for symbol storage. Using this layer
    ///        the content of the symbol storage can be printed.
    ///
    /// Using the trace(std::ostream&,const Filter&) function, the
    ///    byte values of the symbol storage will be dumped to the
    ///    ostream in a human readable format.
    ///
    ///     0 I:
    ///     0000  c6 04 14 7d *** 9c 30 b7 dc  ...} *** .0..
    ///     ....
    ///
    ///     1 A:
    ///     0000  e4 9a ac 91 *** da 6b 4e cf  .... *** .kN.
    ///     ....
    ///
    ///     2 ?: -
    ///
    /// The lines should be interpret as in the following example:
    ///
    ///   +--> 1 A:
    ///   |    0000  e4 9a ac 91 *** da 6b 4e cf  .... *** .kN.
    ///   |                      ^
    ///   |       +--------------+
    ///   |       |
    ///   |       |
    ///   |       +  Hex dump of the symbol's storage
    ///   |
    ///   |
    ///   +--------+  The symbol's index position & storage state
    ///
    /// A symbol's storage can be in 3 states:
    ///
    /// A: The storage is available, but has not necessarily been initialized.
    /// I: The storage is available and has been explicitly been initialized
    ///    by the user through a set_symbol() or set_symbols() call.
    /// ?: No storage is available for this symbol.
    ///
    template<class SuperCoder>
    class trace_symbol_storage<enable_trace, SuperCoder> : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// @copydoc layer::trace(std::ostream&, const Filter&)
        template<class Filter>
        void trace(std::ostream& out, const Filter& filter)
        {
            if (filter("symbol_storage"))
            {
                out << "symbol_storage:" << std::endl;
                print_storage(out);
                out << std::endl;
            }

            // If the lower layers define the trace function forward it
            if (kodo::has_trace<SuperCoder>::value)
            {
                SuperCoder& next = *this;
                kodo::trace(next, out, filter);
            }
        }

    protected:

        /// Print all symbols stored in the storage where the symbol data
        /// is shown in bytes.
        /// @param out The output stream to print to
        void print_storage(std::ostream& out) const
        {
            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                print_symbol(out, i);
            }
        }

        /// Print the bytes of a symbol at a specific index to the
        /// output stream
        /// @param out The output stream to print to
        /// @param index The index of symbol to print
        void print_symbol(std::ostream& out, uint32_t index) const
        {
            assert(index < SuperCoder::symbols());

            print_symbol_info(out, index);

            uint32_t size = SuperCoder::symbol_size();
            if(SuperCoder::is_symbol_available(index))
            {
                const uint8_t* symbol = SuperCoder::symbol(index);
                auto storage = sak::storage(symbol, size);

                hexdump hex(storage);
                hex.set_max_size(32);

                out << hex;
            }

        }

        /// Print the bytes of a symbol at a specific index to the
        /// output stream
        /// @param out The output stream to print to
        /// @param index The index of symbol to print
        void print_symbol_info(std::ostream& out, uint32_t index) const
        {
            assert(index < SuperCoder::symbols());

            if(SuperCoder::is_symbol_available(index))
            {
                if(SuperCoder::is_symbol_initialized(index))
                {
                    out << index << " I:  " << std::endl;
                }
                else
                {
                    out << index << " A:  " << std::endl;
                }

            }
            else
            {
                out << index << " ?:  -" << std::endl;
            }
        }


    };
}
