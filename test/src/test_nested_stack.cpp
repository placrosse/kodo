// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <memory>

#include <gtest/gtest.h>

#include <kodo/basic_nested_stack.hpp>
#include "kodo_unit_test/helper_test_nested_stack.hpp"

TEST(TestNestedStack, api)
{
    uint32_t symbols = 16;
    uint32_t symbol_size = 1400;

    kodo::helper_test_nested_stack::factory factory(symbols, symbol_size);
    EXPECT_EQ(factory.m_max_symbols, symbols);
    EXPECT_EQ(factory.m_max_symbol_size, symbol_size);

    auto& nested_factory = factory.nested();
    EXPECT_EQ(nested_factory.m_max_symbols, symbols);
    EXPECT_EQ(nested_factory.m_max_symbol_size, symbol_size);

    kodo::helper_test_nested_stack stack;

    stack.initialize(factory);

    auto nested = stack.nested();
    assert(nested);

    EXPECT_EQ(nested->m_symbols, symbols);
    EXPECT_EQ(nested->m_symbol_size, symbol_size);

    // Try to adjust the symbols and symbol size. If we now initialize
    // the stack again we should see the change refelected in the
    // nested stack
    factory.m_symbols = 10;
    factory.m_symbol_size = 100;

    stack.initialize(factory);

    nested = stack.nested();
    assert(nested);

    EXPECT_EQ(nested->m_symbols, factory.m_symbols);
    EXPECT_EQ(nested->m_symbol_size, factory.m_symbol_size);
}
