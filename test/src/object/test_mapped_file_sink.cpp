// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_mapped_file_sink.cpp Unit tests for the
///       mapped_file_sink class

#include <cstdint>
#include <memory>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/object/mapped_file_sink.hpp>

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

        struct factory_base
        {
            factory_base(uint32_t symbols, uint32_t symbol_size)
            {
                (void) symbols;
                (void) symbol_size;
            }

            std::string file_name() const
            {
                return m_filename();
            }

            void set_storage(const sak::mutable_storage& storage)
            {
                m_set_storage(storage);
            }

            stub::call<std::string()> m_filename;
            stub::call<void(const sak::mutable_storage&)> m_set_storage;
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
        }

        template<class Factory>
        void deinitialize(Factory& the_factory)
        {
            (void) the_factory;
        }

    };

    // Helper stack
    class dummy_stack : public object::mapped_file_sink<dummy_layer>
    { };
}
}

/// @todo figure out is this is the right naming
TEST(ObjectTestMappedFileSink, api)
{
    uint32_t symbols = 10;
    uint32_t symbol_size = 100;

    kodo::dummy_stack stack;
    kodo::dummy_stack::factory_base factory(symbols, symbol_size);

    factory.m_filename.set_return("mapped_file_sink_one");
    factory.set_file_size(1234);

    EXPECT_EQ(factory.file_size(), 1234);

    // Check that we are fine
    stack.deinitialize(factory);

    stack.initialize(factory);

    // Comparison function which checks the size of the storage object
    // and that the data pointer is not null
    auto compare = [](const std::tuple<sak::mutable_storage>& actual,
                      const std::tuple<sak::mutable_storage>& expected) -> bool
        {
            auto actual_storage = std::get<0>(actual);
            auto expected_storage = std::get<0>(expected);

            if (actual_storage.m_size != expected_storage.m_size)
                return false;

            if (actual_storage.m_data == 0)
                return false;

            if (expected_storage.m_data != (uint8_t*)0xdeadbeef)
                return false;

            return true;
        };

    sak::mutable_storage check_one = {(uint8_t*)0xdeadbeef, 1234};
    EXPECT_TRUE((bool) factory.m_set_storage.expect_calls(compare)
                    .with(check_one));

    // Initialize again
    factory.m_filename.set_return("mapped_file_sink_two");
    factory.set_file_size(4321);

    EXPECT_EQ(factory.file_size(), 4321);
    stack.initialize(factory);

    sak::mutable_storage check_two = {(uint8_t*)0xdeadbeef, 4321};
    EXPECT_TRUE((bool) factory.m_set_storage.expect_calls(compare)
                    .with(check_one)
                    .with(check_two));

    stack.deinitialize(factory);
}
