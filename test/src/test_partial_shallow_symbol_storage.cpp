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

            void set_symbol(uint32_t index,
                            const sak::const_storage &symbol_storage)
            {
                return m_set_symbol(index, symbol_storage);
            }

            stub::call<void()> m_initialize;
            stub::call<void()> m_construct;

            stub::call<uint32_t()> m_symbol_size;
            stub::call<uint32_t()> m_symbols;
            stub::call<void(uint32_t, sak::const_storage)> m_set_symbol;
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

    stack.construct(factory);
    stack.initialize(factory);

    // This is initialize in the dummy layer using the factory
    EXPECT_EQ(stack.symbol_size(), 5U);

    // Check state is correct and that initialize and construct calls
    // were correctly forwarded
    EXPECT_TRUE(stack.m_construct.called_once_with());
    EXPECT_TRUE(stack.m_initialize.called_once_with());
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
    using parameter = std::tuple<uint32_t, sak::const_storage>;

    auto p = [](const parameter& a, const parameter& b) -> bool
        {
            if(std::get<0>(a) != std::get<0>(b))
                return false;

            if(sak::is_same(std::get<1>(a), std::get<1>(b)))
                return true;

            return false;
        };


    // The actual calls that were made
    std::vector<parameter> real_calls =
        {
            std::make_tuple(0, sak::const_storage(data.data() + 0, 5U)),
            std::make_tuple(1, sak::const_storage(data.data() + 5, 5U)),
            std::make_tuple(2, sak::const_storage(data.data() + 10, 5U)),
            std::make_tuple(3, sak::const_storage(data.data() + 15, 5U)),
            std::make_tuple(4, sak::const_storage(data.data() + 20, 5U))
        };

    EXPECT_TRUE(stack.m_set_symbol.has_calls(real_calls, p));
}

// Test that the stack functions properly when the partial symbol is
// needed
TEST(TestPartialShallowSymbolStorage, partial)
{
    kodo::dummy_factory factory;

    factory.m_max_symbol_size.set_return(10U);
    factory.m_symbol_size.set_return(5U);

    kodo::dummy_stack stack;

    stack.construct(factory);
    stack.initialize(factory);

    // This is initialize in the dummy layer using the factory
    EXPECT_EQ(stack.symbol_size(), 5U);

    // Check state is correct and that initialize and construct calls
    // were correctly forwarded
    EXPECT_TRUE(stack.m_construct.called_once_with());
    EXPECT_TRUE(stack.m_initialize.called_once_with());
    EXPECT_FALSE(stack.has_partial_symbol());

    // Set the remaining needed state in the stack
    stack.m_symbols.set_return(5U);

    // Make a buffer that is 23 bytes. That means we will have 4
    // symbols of 5 bytes and one symbol which only be 3 bytes.
    std::vector<uint8_t> data(23U);

    stack.set_symbols(sak::storage(data));
    EXPECT_TRUE(stack.has_partial_symbol());
    EXPECT_EQ(stack.partial_symbol_size(), 3U);

    const uint8_t *partial_data = stack.partial_symbol();
    EXPECT_TRUE(partial_data != 0);

    // To check that that the calls made to the set_symbol functions
    // are what we expect we need a custom predicate function to
    // compare the arguments. The reason is that we need to use the
    // is_same function to compare the sak::const_storage
    // objects. is_same compares that the pointers point to the same
    // memory as opposed to just checking whether the content is equal
    using parameter = std::tuple<uint32_t, sak::const_storage>;

    auto p = [](const parameter& a, const parameter& b) -> bool
        {
            if(std::get<0>(a) != std::get<0>(b))
                return false;

            if(sak::is_same(std::get<1>(a), std::get<1>(b)))
                return true;

            return false;
        };


    // The actual calls that were made
    std::vector<parameter> real_calls =
        {
            std::make_tuple(0, sak::const_storage(data.data() + 0, 5U)),
            std::make_tuple(1, sak::const_storage(data.data() + 5, 5U)),
            std::make_tuple(2, sak::const_storage(data.data() + 10, 5U)),
            std::make_tuple(3, sak::const_storage(data.data() + 15, 5U)),
            std::make_tuple(4, sak::const_storage(partial_data, 5U))
        };

    EXPECT_TRUE(stack.m_set_symbol.has_calls(real_calls, p));
}
