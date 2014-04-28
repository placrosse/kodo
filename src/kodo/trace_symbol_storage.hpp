// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>

#include <fifi/fifi_utils.hpp>

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
    /// This layer provides two different ways of printing the symbol
    /// data:
    /// 1) Using the print_storage() and print_symbol() functions, the
    ///    data will be printed using the numeric value of the bytes stored
    /// 2) Using the print_storage_value() and print_symbol_value() functions,
    ///    the data will be printed using the numeric value of the finite field
    ///    used
    ///
    /// 0 I:  23 35 222 35 0 10
    /// 1 A:  33 22 19 01 10 15
    /// 2 ?:  -
    ///
    /// The lines should be interpret as in the following example:
    ///
    ///   1 A:  33 22 19 01 10 15
    ///   ^ ^   ^
    ///   | |   |
    ///   | |   |
    ///   | |   +--+  The values of the symbol storage (format depends on
    ///   | |          the print function used either bytes or finite field)
    ///   | +------+  The symbol state
    ///   +--------+  The symbol's pivot position
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

    protected:

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
