// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>
#include <iomanip>

#include <fifi/fifi_utils.hpp>

#include "enable_trace.hpp"
#include "disable_trace.hpp"
#include "trace.hpp"

namespace kodo
{

    /// Fall-through case for the case where TraceTag is disable_trace
    template<class TraceTag, class SuperCoder>
    class trace_linear_block_decoder : public SuperCoder
    {
        static_assert(std::is_same<TraceTag, disable_trace>::value,
                      "Unexpected TraceTag should be disable_trace in the "
                      "fall-through case.");
    };

    /// @ingroup trace_layers
    ///
    /// @brief Trace layer which allows inspecting the state of a
    ///        linear block decoder.
    ///
    /// The trace layer will print out the coding coefficients used by
    /// the decoder, when the print_decoder_state() function is called.
    ///
    /// This will print a matrix like the following (in the following
    /// case we have 3 symbols):
    ///
    /// 0 U:  1 0 0
    /// 1 C:  0 1 1
    /// 2 ?:  0 0 0
    ///
    /// The first line should be interpret as follows:
    ///
    ///   0 U:  1 0 0
    ///   ^ ^   ^ ^ ^
    ///   | |   | | +-----+  3rd coding coefficient
    ///   | |   | +-------+  2nd coding coefficient
    ///   | |   +---------+  1st coding coefficient
    ///   | +-------------+  The symbol state
    ///   +---------------+  The symbol's pivot position
    ///
    /// A symbol can be in 3 states:
    ///
    /// U: The symbol is uncoded (i.e. only one non zero coefficient should
    ///    be in the coding coefficients.
    /// C: The symbol is coded
    /// ?: No symbol for this position in the decoder has been see yet.
    ///
    /// The coding coefficient values will depend on the chosen finite
    /// field
    template<class SuperCoder>
    class trace_linear_block_decoder<enable_trace, SuperCoder> :
        public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

    public:

        /// @copydoc layer::trace(std::ostream&)
        void trace(std::ostream& out)
        {
            print_decoder_state(out);

            // If the lower layers define the trace function forward it
            if (kodo::has_trace<SuperCoder>::value)
            {
                SuperCoder& next = *this;
                kodo::trace(next, out);
            }

        }

        /// Prints the decoder's state to the output stream
        /// @param out, the output stream
        void print_decoder_state(std::ostream &out) const
        {
            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                if (SuperCoder::is_symbol_missing(i))
                {
                    out << std::setfill(' ') << std::setw(3) << i << " ?:  ";
                }
                else if (SuperCoder::is_symbol_seen(i))
                {
                    out << std::setfill(' ') << std::setw(3) << i << " C:  ";
                }
                else
                {
                    assert(SuperCoder::is_symbol_uncoded(i));
                    out << std::setfill(' ') << std::setw(3) << i << " U:  ";
                }

                const value_type* c = SuperCoder::coefficient_vector_values(i);

                for(uint32_t j = 0; j < SuperCoder::coefficient_vectors(); ++j)
                {

                    static_assert(sizeof(uint32_t) >= sizeof(value_type),
                                  "value_type will overflow in this print");

                    value_type value = SuperCoder::coefficient_value(c, j);
                    out << (uint32_t)value << " ";
                }

                out << std::endl;
            }

        }

    };

}
