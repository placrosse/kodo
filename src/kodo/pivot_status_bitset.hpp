// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <boost/dynamic_bitset.hpp>

namespace kodo
{

    /// @todo document/test
    ///
    /// @ingroup decoder_api status_api
    ///
    /// @brief The pivot status writer keeps track of which symbols
    /// currently have been marked as pivot in the decoding
    /// matrix. This information can then be written using the write
    /// function.
    template<class SuperCoder>
    class pivot_status_bitset : public SuperCoder
    {
    public:

        /// The data used for the status vector
        typedef uint8_t status_block_type;

        /// The bitset type
        boost::dynamic_bitset<status_block_type> status_bitset_type;

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_pivot_status.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            // It is ok to resize here since the dynamic_bitset will
            // not actually shrink its capacity - since we have already
            // ensured that the bitset has the maximum needed size in
            // construct
            m_pivot_status.resize(the_factory.symbols(), false);
        }

        /// @return The size in bytes of decoder status vector
        uint32_t pivot_status_size() const
        {
            static_assert(sizeof(status_block_type) == 1,
                          "We assume that the block type is 1 byte in the "
                          "calculation here");

            return m_pivot_status.num_blocks();
        }

    protected:

        /// Tracks the symbols which have been marked as pivot
        status_bitset_type m_pivot_status;

    };

}

