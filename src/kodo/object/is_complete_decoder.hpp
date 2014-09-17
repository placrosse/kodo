// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <functional>
#include <cassert>
#include <vector>
#include <cstdint>

namespace kodo
{
namespace object
{
    /// @ingroup object_layers
    ///
    /// This layer makes sure that we can track the completion of all the
    /// decoders built.
    ///
    /// This layer expects that a decoder exposes the
    /// set_is_complete_callback(...) which we will use to register a
    /// callback function. This functionality will be available in
    /// stacks using the is_complete_callback_decoder.hpp layer.
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

            m_completed.resize(SuperCoder::blocks());
            fill(begin(m_completed), end(m_completed), false);
        }

        /// @param index Index of the block to build an encoder or
        ///        decoder stack for
        ///
        /// @return the newly built encoder or decoder initialized
        ///         with storage through the the set_symbols function
        auto build(uint32_t index) ->
            decltype(std::declval<SuperCoder>().build(0))
        {
            assert(index < SuperCoder::blocks());

            // We don't allow building decoders already completed
            assert(m_completed[index] == false);

            auto stack = SuperCoder::build(index);
            assert(stack);

            stack->set_is_complete_callback(
                std::bind(&is_complete_decoder::is_complete_callback,
                          this, index));

           return stack;
        }

        /// @return true if all decoders have been decoded
        bool is_complete() const
        {
            return m_completed_count == SuperCoder::blocks();
        }

        /// @param index The index of the block we want to check
        ///        whether is complete
        ///
        /// @return true if the specific block is complete
        bool is_block_complete(uint32_t index) const
        {
            assert(index < SuperCoder::blocks());
            return m_completed[index];
        }

    private:

        /// The callback function which will be invoked by the
        /// different decoders once they complete.
        void is_complete_callback(uint32_t index)
        {
            assert(index < m_completed.size());
            assert(m_completed[index] == false);

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
