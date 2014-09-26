// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{

    /// @ingroup proxy_layers status_api
    ///
    /// @brief The proxy_remote_rank will forward calls to the
    ///        remote_rank() function to a main stack
    template<class SuperCoder>
    class proxy_remote_rank : public SuperCoder
    {
    public:

        /// The type of the main stack
        typedef typename SuperCoder::main_stack_type main_stack_type;

        /// @copydoc layer::rank_type
        typedef typename main_stack_type::rank_type rank_type;

    public:

        /// @copydoc layer::remote_rank() const
        rank_type remote_rank() const
        {
            const auto& main_stack = SuperCoder::main_stack();
            assert(main_stack);

            return main_stack->remote_rank();
        }

    };

}
