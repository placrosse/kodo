// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_is_complete_decoder.cpp Unit tests for the
///       is_complete_decoder class

#include <cstdint>
#include <memory>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/object/is_complete_decoder.hpp>

namespace kodo
{
// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    using callback_type = std::function<void()>;

    /// Helper layer
    class dummy_layer
    {
    public:

        struct stack
        {
            void set_is_complete_callback(const callback_type& callback)
            {
                assert(!m_callback);
                m_callback = callback;
            }

            callback_type m_callback;
        };

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
            m_initialize();
        }

        uint32_t blocks() const
        {
            return m_blocks();
        }

        std::shared_ptr<stack> build(uint32_t index)
        {
            return m_build(index);
        }

    public:

        stub::call<void()> m_initialize;
        stub::call<uint32_t()> m_blocks;
        stub::call<std::shared_ptr<stack>(uint32_t)> m_build;
    };

    // Helper stack
    class dummy_stack : public object::is_complete_decoder<dummy_layer>
    { };

    // Helper factory
    class dummy_factory
    { };
}
}

/// @todo figure out is this is the right naming
TEST(ObjectTestIsCompleteDecoder, api)
{

    kodo::dummy_factory factory;
    kodo::dummy_stack stack;

    stack.m_blocks.set_return(5U);
    stack.m_build.set_return(
        {std::make_shared<kodo::dummy_stack::stack>(),
         std::make_shared<kodo::dummy_stack::stack>(),
         std::make_shared<kodo::dummy_stack::stack>(),
         std::make_shared<kodo::dummy_stack::stack>(),
         std::make_shared<kodo::dummy_stack::stack>()}).no_repeat();

    stack.initialize(factory);

    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());
    EXPECT_FALSE(stack.is_complete());

    EXPECT_FALSE(stack.is_block_complete(0U));
    EXPECT_FALSE(stack.is_block_complete(1U));
    EXPECT_FALSE(stack.is_block_complete(2U));
    EXPECT_FALSE(stack.is_block_complete(3U));
    EXPECT_FALSE(stack.is_block_complete(4U));

    auto stack0 = stack.build(0);
    stack0->m_callback();

    EXPECT_FALSE(stack.is_complete());

    EXPECT_TRUE(stack.is_block_complete(0U));
    EXPECT_FALSE(stack.is_block_complete(1U));
    EXPECT_FALSE(stack.is_block_complete(2U));
    EXPECT_FALSE(stack.is_block_complete(3U));
    EXPECT_FALSE(stack.is_block_complete(4U));

    auto stack1 = stack.build(1);
    stack1->m_callback();

    EXPECT_FALSE(stack.is_complete());

    EXPECT_TRUE(stack.is_block_complete(0U));
    EXPECT_TRUE(stack.is_block_complete(1U));
    EXPECT_FALSE(stack.is_block_complete(2U));
    EXPECT_FALSE(stack.is_block_complete(3U));
    EXPECT_FALSE(stack.is_block_complete(4U));

    auto stack2 = stack.build(2);
    auto stack3 = stack.build(3);
    auto stack4 = stack.build(4);

    stack2->m_callback();
    stack3->m_callback();
    stack4->m_callback();

    EXPECT_TRUE(stack.is_complete());

    EXPECT_TRUE(stack.is_block_complete(0U));
    EXPECT_TRUE(stack.is_block_complete(1U));
    EXPECT_TRUE(stack.is_block_complete(2U));
    EXPECT_TRUE(stack.is_block_complete(3U));
    EXPECT_TRUE(stack.is_block_complete(4U));

    // Try to re-initialize
    stack.m_blocks.set_return(2U);
    stack.m_build.set_return(
        {std::make_shared<kodo::dummy_stack::stack>(),
         std::make_shared<kodo::dummy_stack::stack>()}).no_repeat();

    stack.initialize(factory);

    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with().with());
    EXPECT_FALSE(stack.is_complete());

    EXPECT_FALSE(stack.is_block_complete(0U));
    EXPECT_FALSE(stack.is_block_complete(1U));

    stack0 = stack.build(0);
    stack1 = stack.build(1);

    stack0->m_callback();
    stack1->m_callback();

    EXPECT_TRUE(stack.is_complete());

    EXPECT_TRUE(stack.is_block_complete(0U));
    EXPECT_TRUE(stack.is_block_complete(1U));
}
