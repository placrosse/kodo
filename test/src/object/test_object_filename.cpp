// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_object_filename.cpp Unit tests for the
///       object_filename class

#include <cstdint>
#include <memory>
#include <fstream>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <stub/call.hpp>

#include <kodo/object/object_filename.hpp>

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

        class factory_base
        {
        public:

            factory_base(uint32_t symbols, uint32_t symbol_size)
            {
                (void) symbols;
                (void) symbol_size;
            }
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
        }

    };

    // Helper stack
    class dummy_stack : public object::object_filename<dummy_layer>
    { };
}
}

/// Test that the object_filename layer works as expected
TEST(ObjectTestObjectFilename, api)
{
    kodo::dummy_stack stack;
    kodo::dummy_stack::factory_base factory(10U, 100U);

    auto filename = "yoyo.dat";

    factory.set_filename(filename);
    EXPECT_EQ(factory.filename(), filename);

    stack.initialize(factory);
    EXPECT_EQ(stack.filename(), filename);
}
