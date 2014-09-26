// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @ingroup trace
    ///
    /// @brief Tag used in trace layers and codec stacks to enable traceging
    ///
    /// We use two tags in Kodo to either enable or disable traceging:
    ///   1. enable_trace This tag enables trace functionality
    ///   2. disable_trace This tag disables trace functionality
    ///
    /// The basis usage of these tags can be illustrated though an
    /// example, in the following we want to implement a trace layer
    /// i.e. a layer which enables us to peek inside the stack and
    /// inspect its state.
    ///
    /// The boiler-plate implementation of such a layer would look like this:
    ///
    ///     template<class TraceTag, class Super Coder>
    ///     class trace_this_and_that : public SuperCoder
    ///     {
    ///         static_assert(std::is_same<TraceTag, disable_trace>::value,
    ///           "Unexpected TraceTag should be disable_trace in the "
    ///                  "fall-through case.");
    ///      };
    ///
    ///
    ///     template<class SuperCoder>
    ///     class trace_this_and_that<enable_trace, SuperCoder> :
    ///         public SuperCoder
    ///     {
    ///     public:
    ///
    ///         tempalte<class Filter>
    ///         void trace(std::ostream& out, const Filter&)
    ///         {
    ///              // print some useful information
    ///         }
    ///     };
    ///
    /// Notice how we have two template classes one generic template
    /// and one specialization. The specialization kicks in when the
    /// TraceTag equaled enable_trace. In that case the
    /// trace_this_and_that will expose the
    /// layer::trace(std::ostream&, const Filter&) function to the user of the
    /// stack. In the generic fall-through case no
    /// layer::trace(std::ostream&, const Filter&) function would be exposed.
    ///
    struct enable_trace{};
}
