// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_nested_catch_all.cpp Unit tests for the nested catch all

#include <gtest/gtest.h>

#include <kodo/nested_catch_all.hpp>

// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{

    // This layer will provide all the functions invoked by the
    // nested_catch_all layer so that we can check that all calls are
    // correctly forwarded.
    class dummy_layer
    {
    public:

        // Three typedefs are required

    public:

        class factory
        {
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
            { }
        };

    public:



    };
}

/// Run the tests typical coefficients stack
TEST(TestNestedCatchAll, api)
{
    ///  @todo implement test
}
