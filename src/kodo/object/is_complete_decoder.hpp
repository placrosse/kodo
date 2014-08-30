// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <functional>
#include <cassert>
#include <vector>

#include <iostream> /// @todo delete

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
    class is_complete_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_completed_count = 0;

            m_completed.resize(SuperCoder::blocks(), false);
        }

        /// @param index Index of the block to build an encoder or
        ///        decoder stack for
        ///
        /// @return the newly built encoder or decoder initialized
        ///         with storage through the the set_symbols function
        auto build(uint32_t index) ->
            decltype(std::declval<SuperCoder>().build(0))
        {
            auto stack = SuperCoder::build(index);
            assert(stack);

            stack->set_is_complete_callback(
                std::bind(&is_complete_decoder::is_complete_callback, this, index));

            return stack;
        }

        ///
        bool is_complete() const
        {
            return m_completed_count == SuperCoder::blocks();
        }

        ///
        bool is_block_complete(uint32_t index) const
        {
            assert(index < SuperCoder::blocks());
            return m_completed[index];
        }

    private:

        void is_complete_callback(uint32_t index)
        {
            assert(index < m_completed.size());
            assert(m_completed[index] == false);

            std::cout << "Index complete " << index << std::endl;
            ++m_completed_count;
            assert(m_completed_count <= m_completed.size());
            m_completed[index] = true;
        }

    private:

        /// Keeps track of the number of completed blocks
        uint32_t m_completed_count;

        /// Keeps track of which of the blocks are completed
        std::vector<bool> m_completed;
    };
}
}
