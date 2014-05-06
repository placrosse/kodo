// Copyright Steinwurf ApS 2011-2013
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @todo docs + tests
    ///
    /// @brief This layer "adjusts" the rank seen by the user. This is
    ///        needed since the inner decoder in reality has to
    ///        decoder symbols + expansion but from the users
    ///        perspective the rank is expected to go from 0 to
    ///        symbols.
    ///
    /// In this layer we return rank 0 as long as the rank has not
    /// increased above the expansion size. This means that symbols
    /// contributing to decoding the expansion might be interpret as
    /// linear dependent (which is in some sense is both true and
    /// false).
    ///
    /// In any case the main purpose of this layer is to have the rank
    /// increase as an user would logically expect namely from 0 to
    /// symbols (and when rank == symbols decoding should be
    /// complete).
    template<class SuperCoder>
    class fulcrum_rank_inner_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::rank() const
        uint32_t rank() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);

            uint32_t rank = nested->rank();
            uint32_t expansion = SuperCoder::expansion();

            if(rank < expansion)
            {
                return 0;
            }
            else
            {
                return rank - expansion;
            }
        }
    };
}
