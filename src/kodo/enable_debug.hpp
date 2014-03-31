// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @ingroup debug
    ///
    /// @brief Tag used in debug layers and codec stacks to enable debugging
    ///
    /// We use two tags in Kodo to either enable or disable debugging:
    ///   1. enable_debug This tag enables debug functionality
    ///   2. disable_debug This tag disables debug functionality
    ///
    /// The basis usage of these tags can be illustrated though an
    /// example, in the following we want to implement a debug layer
    /// i.e. a layer which enables us to peek inside the stack and
    /// inspect its state.
    ///
    /// The boiler-plate implementation of such a layer would look like this:
    ///
    ///     template<class DebugTag, class Super Coder>
    ///     class debug_this_and_that : public SuperCoder
    ///     {
    ///         static_assert(std::is_same<DebugTag, disable_debug>::value,
    ///           "Unexpected DebugTag should be disable_debug in the "
    ///                  "fall-through case.");
    ///      };
    ///
    ///
    ///     template<class SuperCoder>
    ///     class debug_this_and_that<enable_debug, SuperCoder> :
    ///         public SuperCoder
    ///     {
    ///     public:
    ///
    ///         void debug(std::ostream& out)
    ///         {
    ///              // print some useful information
    ///         }
    ///     };
    ///
    /// Notice how we have two template classes one generic template
    /// and one specialization. The specialization kicks in when the
    /// DebugTag equaled enable_debug. In that case the
    /// debug_this_and_that will expose the
    /// layer::debug(std::ostream&) function to the user of the
    /// stack. In the generic fall-through case no
    /// layer::debug(std::ostream&) function would be exposed.
    ///
    struct enable_debug{};
}



