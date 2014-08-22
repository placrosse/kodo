// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_partial_shallow_symbol_storage.cpp Unit tests for the
///       partial_shallow_symbol_storage class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/partial_shallow_symbol_storage.hpp>

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

            using storage_type = sak::mutable_storage;

        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
                m_construct();
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_initialize();
                m_symbol_size.set_return(the_factory.symbol_size());
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size();
            }

            uint32_t symbols() const
            {
                return m_symbols();
            }

            void set_symbol(uint32_t index, const storage_type &symbol_storage)
            {
                return m_set_symbol(index, symbol_storage);
            }

            stub::call<void()> m_initialize;
            stub::call<void()> m_construct;

            stub::call<uint32_t()> m_symbol_size;
            stub::call<uint32_t()> m_symbols;
            stub::call<void(uint32_t, storage_type)> m_set_symbol;
        };

        // Helper stack
        class dummy_stack : public partial_shallow_symbol_storage<dummy_layer>
        { };

        // Helper factory
        class dummy_factory
        {
        public:

            uint32_t max_symbol_size() const
            {
                return m_max_symbol_size();
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size();
            }


            stub::call<uint32_t()> m_max_symbol_size;
            stub::call<uint32_t()> m_symbol_size;
        };
    }
}

// Test that the stack functions properly when the partial symbol is
// not needed
TEST(TestPartialShallowSymbolStorage, no_partial)
{
    kodo::dummy_factory factory;

    factory.m_max_symbol_size.set_return(10U);
    factory.m_symbol_size.set_return(5U);

    kodo::dummy_stack stack;
    using storage_type = kodo::dummy_stack::storage_type;

    stack.construct(factory);
    stack.initialize(factory);

    // This is initialize in the dummy layer using the factory
    EXPECT_EQ(stack.symbol_size(), 5U);

    // Check state is correct and that initialize and construct calls
    // were correctly forwarded
    EXPECT_TRUE((bool) stack.m_construct.expect_calls().with());
    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());
    EXPECT_FALSE(stack.has_partial_symbol());

    // Set the remaining needed state in the stack
    stack.m_symbols.set_return(5U);

    // Make a buffer that perfectly fits 5 symbols of size 5 bytes
    std::vector<uint8_t> data(25U);

    stack.set_symbols(sak::storage(data));
    EXPECT_FALSE(stack.has_partial_symbol());

    // To check that that the calls made to the set_symbol functions
    // are what we expect we need a custom predicate function to
    // compare the arguments. The reason is that we need to use the
    // is_same function to compare the sak::const_storage
    // objects. is_same compares that the pointers point to the same
    // memory as opposed to just checking whether the content is equal
    using parameter = std::tuple<uint32_t, storage_type>;

    auto p = [](const parameter& a, const parameter& b) -> bool
        {
            if(std::get<0>(a) != std::get<0>(b))
                return false;

            if(sak::is_same(std::get<1>(a), std::get<1>(b)))
                return true;

            return false;
        };

    EXPECT_TRUE((bool) stack.m_set_symbol.expect_calls(p)
              .with(0, storage_type(data.data() + 0, 5U))
              .with(1, storage_type(data.data() + 5, 5U))
              .with(2, storage_type(data.data() + 10, 5U))
              .with(3, storage_type(data.data() + 15, 5U))
              .with(4, storage_type(data.data() + 20, 5U)));

}

// Test that the stack functions properly when the partial symbol is
// needed
TEST(TestPartialShallowSymbolStorage, partial)
{
    kodo::dummy_factory factory;

    factory.m_max_symbol_size.set_return(10U);
    factory.m_symbol_size.set_return(5U);

    kodo::dummy_stack stack;
    using storage_type = kodo::dummy_stack::storage_type;

    stack.construct(factory);
    stack.initialize(factory);

    // This is initialize in the dummy layer using the factory
    EXPECT_EQ(stack.symbol_size(), 5U);

    // Check state is correct and that initialize and construct calls
    // were correctly forwarded
    EXPECT_TRUE((bool) stack.m_construct.expect_calls().with());
    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());
    EXPECT_FALSE(stack.has_partial_symbol());

    // Set the remaining needed state in the stack
    stack.m_symbols.set_return(5U);

    // Make a buffer that is 23 bytes. That means we will have 4
    // symbols of 5 bytes and one symbol which only be 3 bytes.
    std::vector<uint8_t> data(23U, 'a');

    stack.set_symbols(sak::storage(data));
    EXPECT_TRUE(stack.has_partial_symbol());

    // To check that that the calls made to the set_symbol functions
    // are what we expect we need a custom predicate function to
    // compare the arguments. The reason is that we need to use the
    // is_same function to compare the sak::const_storage
    // objects. is_same compares that the pointers point to the same
    // memory as opposed to just checking whether the content is equal
    using parameter = std::tuple<uint32_t, storage_type>;

    auto compare = [](const parameter& a, const parameter& b) -> bool
        {
            if(std::get<0>(a) != std::get<0>(b))
                return false;

            if(sak::is_same(std::get<1>(a), std::get<1>(b)))
                return true;

            return false;
        };

    EXPECT_EQ(stack.m_set_symbol.calls(), 5U);

    // The actual calls that were made
    auto zero = std::make_tuple(0, storage_type(data.data() + 0, 5U));
    auto one = std::make_tuple(1, storage_type(data.data() + 5, 5U));
    auto two = std::make_tuple(2, storage_type(data.data() + 10, 5U));
    auto three = std::make_tuple(3, storage_type(data.data() + 15, 5U));
    auto four = std::make_tuple(4, storage_type(data.data() + 20, 5U));

    EXPECT_TRUE(compare(stack.m_set_symbol.call_arguments(0), zero));
    EXPECT_TRUE(compare(stack.m_set_symbol.call_arguments(1), one));
    EXPECT_TRUE(compare(stack.m_set_symbol.call_arguments(2), two));
    EXPECT_TRUE(compare(stack.m_set_symbol.call_arguments(3), three));
    EXPECT_FALSE(compare(stack.m_set_symbol.call_arguments(4), four));

    // Test the restore (should bring back the a's)
    data[20] = 'b';
    data[21] = 'c';
    data[22] = 'd';

    stack.restore_partial_symbol();

    EXPECT_TRUE(data[20] == 'a');
    EXPECT_TRUE(data[21] == 'a');
    EXPECT_TRUE(data[22] == 'a');

}
