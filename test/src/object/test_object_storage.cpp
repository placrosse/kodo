// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_object_storage.cpp Unit tests for the
///       object_storage class

#include <memory>

#include <gtest/gtest.h>
#include <stub/call.hpp>
#include <sak/storage.hpp>

#include <kodo/object/object_storage.hpp>

namespace kodo
{
// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    /// Helper layer
    class dummy_layer
    {
    public:

        /// The stack we will build
        class stack
        {
        public:

            using storage_type = sak::const_storage;

            void set_symbols(const storage_type& storage)
            {
                m_set_symbols(storage);
            }

        public:

            stub::call<void(storage_type)> m_set_symbols;
        };

        using stack_type = stack;

    public:

        /// The factory_base needed
        class factory_base
        {
        public:

            factory_base(uint32_t symbols, uint32_t symbol_size)
            {
                m_constructor(symbols, symbol_size);
            }

        public:

            stub::call<void(uint32_t,uint32_t)> m_constructor;
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
            m_initialize();
        }

        uint32_t byte_offset(uint32_t index) const
        {
            return m_byte_offset(index);
        }

        uint32_t block_size(uint32_t index) const
        {
            return m_block_size(index);
        }

        std::shared_ptr<stack> build(uint32_t index)
        {
            return m_build(index);
        }

    public:

        stub::call<void()> m_initialize;
        stub::call<uint32_t(uint32_t)> m_byte_offset;
        stub::call<uint32_t(uint32_t)> m_block_size;
        stub::call<std::shared_ptr<stack>(uint32_t)> m_build;
    };

    // Helper stack
    class dummy_stack : public object::object_storage<dummy_layer>
    { };
}
}

/// Test the object storage class sets the correct storage objects
/// when the data fits perfectly
TEST(ObjectTestObjectStorage, data_fits)
{
    // Setup the test data
    uint32_t symbols = 5;
    uint32_t symbol_size = 100;
    uint32_t blocks = 5;

    uint32_t data_size = symbols * symbol_size * blocks;

    std::vector<uint8_t> data(data_size);

    // Check the factory
    kodo::dummy_stack::factory_base factory(symbols, symbol_size);
    factory.set_storage(sak::storage(data));

    EXPECT_TRUE(sak::is_same(factory.storage(), sak::storage(data)));
    EXPECT_EQ(factory.object_size(), data_size);
    EXPECT_TRUE((bool)factory.m_constructor.expect_calls()
                    .with(symbols, symbol_size));

    // Setup the stack
    kodo::dummy_stack stack;

    auto stack0 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack1 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack2 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack3 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack4 = std::make_shared<kodo::dummy_stack::stack>();

    stack.m_byte_offset.set_return(
        {0U, 500U, 1000U, 1500U, 2000U}).no_repeat();

    stack.m_block_size.set_return(
        {500U, 500U, 500U, 500U, 500U}).no_repeat();

    stack.m_build.set_return({stack0, stack1, stack2, stack3, stack4})
        .no_repeat();

    stack.initialize(factory);

    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());

    auto b0 = stack.build(0);
    auto b1 = stack.build(1);
    auto b2 = stack.build(2);
    auto b3 = stack.build(3);
    auto b4 = stack.build(4);

    EXPECT_TRUE(stack0 == b0);
    EXPECT_TRUE(stack1 == b1);
    EXPECT_TRUE(stack2 == b2);
    EXPECT_TRUE(stack3 == b3);
    EXPECT_TRUE(stack4 == b4);

    EXPECT_TRUE((bool) stack.m_byte_offset.expect_calls()
                    .with(0)
                    .with(1)
                    .with(2)
                    .with(3)
                    .with(4));

    EXPECT_TRUE((bool) stack.m_block_size.expect_calls()
                    .with(0)
                    .with(1)
                    .with(2)
                    .with(3)
                    .with(4));

    auto compare = [](const std::tuple<sak::const_storage> &a,
                      const std::tuple<sak::const_storage> &b) -> bool
        {
            return sak::is_same(std::get<0>(a), std::get<0>(b));
        };

    EXPECT_TRUE((bool) b0->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data(), 500)));


    EXPECT_TRUE((bool) b1->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 500, 500)));

    EXPECT_TRUE((bool) b2->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 1000, 500)));

    EXPECT_TRUE((bool) b3->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 1500, 500)));

    EXPECT_TRUE((bool) b4->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 2000, 500)));
}

/// Test the object storage class sets the correct storage objects
/// when the data fits perfectly
TEST(ObjectTestObjectStorage, data_does_not_fit)
{
    // Setup the test data
    uint32_t symbols = 5;
    uint32_t symbol_size = 100;
    uint32_t blocks = 5;

    uint32_t data_size = symbols * symbol_size * blocks;

    // Lets reduce the data size so that it is not a multiple of the
    // symbol size. Just as a note in general it does not make sense
    // to reduce the data size with more than the symbol size. Because
    // if that was the case we could simply have on less symbol.
    data_size -= 50;

    std::vector<uint8_t> data(data_size);

    // Check the factory
    kodo::dummy_stack::factory_base factory(symbols, symbol_size);
    factory.set_storage(sak::storage(data));

    EXPECT_TRUE(sak::is_same(factory.storage(), sak::storage(data)));
    EXPECT_EQ(factory.object_size(), data_size);
    EXPECT_TRUE((bool)factory.m_constructor.expect_calls()
                    .with(symbols, symbol_size));

    // Setup the stack
    kodo::dummy_stack stack;

    auto stack0 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack1 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack2 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack3 = std::make_shared<kodo::dummy_stack::stack>();
    auto stack4 = std::make_shared<kodo::dummy_stack::stack>();

    stack.m_byte_offset.set_return(
        {0U, 500U, 1000U, 1500U, 2000U}).no_repeat();

    stack.m_block_size.set_return(
        {500U, 500U, 500U, 500U, 500U}).no_repeat();

    stack.m_build.set_return({stack0, stack1, stack2, stack3, stack4})
        .no_repeat();

    stack.initialize(factory);

    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());

    auto b0 = stack.build(0);
    auto b1 = stack.build(1);
    auto b2 = stack.build(2);
    auto b3 = stack.build(3);
    auto b4 = stack.build(4);

    EXPECT_TRUE(stack0 == b0);
    EXPECT_TRUE(stack1 == b1);
    EXPECT_TRUE(stack2 == b2);
    EXPECT_TRUE(stack3 == b3);
    EXPECT_TRUE(stack4 == b4);

    EXPECT_TRUE((bool) stack.m_byte_offset.expect_calls()
                    .with(0)
                    .with(1)
                    .with(2)
                    .with(3)
                    .with(4));

    EXPECT_TRUE((bool) stack.m_block_size.expect_calls()
                    .with(0)
                    .with(1)
                    .with(2)
                    .with(3)
                    .with(4));

    auto compare = [](const std::tuple<sak::const_storage> &a,
                      const std::tuple<sak::const_storage> &b) -> bool
        {
            return sak::is_same(std::get<0>(a), std::get<0>(b));
        };

    EXPECT_TRUE((bool) b0->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data(), 500)));


    EXPECT_TRUE((bool) b1->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 500, 500)));

    EXPECT_TRUE((bool) b2->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 1000, 500)));

    EXPECT_TRUE((bool) b3->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 1500, 500)));

    // The last coding block get a storage object which is not a
    // multiple of the symbol size and number of symbols.
    EXPECT_TRUE((bool) b4->m_set_symbols.expect_calls(compare)
                    .with(sak::storage(data.data() + 2000, 450)));
}
