// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
namespace object
{
    /// @todo docs + tests
    ///
    /// The codec factory can be shared between the between different
    /// object encoders/decoders. One codec factory is created by the
    /// object factory and shared with all object stacks. So every
    /// object should configure the stack factory before building a
    /// stack with it.
    ///
    /// For this reason we just initialize the callback
    ///
    template<class SuperCoder>
    class is_complete : public SuperCoder
    {
    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_bytes_used = 0;
        }

        /// @copydoc layer::set_bytes_used(uint32_t)
        void set_bytes_used(uint32_t bytes_used)
        {
            assert(bytes_used > 0);
            assert(bytes_used <= SuperCoder::block_size());

            m_bytes_used = bytes_used;
        }

        /// @copydoc layer::bytes_used() const
        uint32_t bytes_used() const
        {
            return m_bytes_used;
        }

    protected:

        /// The number of bytes used
        uint32_t m_bytes_used;
    };
}
}
