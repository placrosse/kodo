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

#include <kodo/object_storage.hpp>

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
        class dummy_stack : public
            object_storage<dummy_layer>
        { };
    }
}

/// Test the object storage class sets the correct storage objects
/// when the data fits perfectly
TEST(TestObjectStorage, data_fits)
{
    // Setup the test data
    uint32_t symbols = 5;
    uint32_t symbol_size = 100;

    std::vector<uint8_t> data(symbols * symbol_size);

    // Check the factory
    kodo::dummy_stack::factory_base factory(symbols, symbol_size);
    factory.set_storage(sak::storage(data));

    EXPECT_TRUE(sak::is_same(factory.storage(), sak::storage(data)));
    EXPECT_EQ(factory.object_size(), symbols * symbol_size);
    EXPECT_TRUE((bool)factory.m_constructor.expect_calls()
                    .with(symbols, symbol_size));

    // Setup the stack
    kodo::dummy_stack stack;

    stack.m_byte_offset.set_return({0U, 100U, 200U, 300U, 400U}).no_repeat();
    stack.m_block_size.set_return({500U, 500U, 500U, 500U, 500U}).no_repeat();








    stack.initialize(factory);

    // auto object_stack = factory.build();


    // stack.m_block_size.set_return(10U);

    // stack.initialize(factory);
    // stack.set_bytes_used(9U);

    // EXPECT_EQ(stack.m_initialize.calls(), 1U);
    // EXPECT_TRUE(stack.m_block_size.called_once_with());
    // EXPECT_EQ(stack.bytes_used(), 9U);

    // // Initialize again and check that the state is reset
    // stack.initialize(factory);
    // EXPECT_EQ(stack.m_initialize.calls(), 2U);
    // EXPECT_TRUE(stack.m_block_size.called_once_with());
    // EXPECT_EQ(stack.bytes_used(), 0U);

}
