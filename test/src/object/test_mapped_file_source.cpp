// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_mapped_file_source.cpp Unit tests for the
///       mapped_file_source class

#include <cstdint>
#include <memory>
#include <fstream>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <stub/call.hpp>

#include <kodo/object/mapped_file_source.hpp>

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


    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            (void) the_factory;
        }

    };

    class dummy_factory
    {
    public:

        std::string filename() const
        {
            return m_filename();
        }

        void set_storage(const sak::const_storage& storage)
        {
            m_set_storage(storage);
        }

        stub::call<std::string()> m_filename;
        stub::call<void(const sak::const_storage&)> m_set_storage;
    };

    // Helper stack
    class dummy_stack : public object::mapped_file_source<dummy_layer>
    { };
}
}

/// Try to open a file which does not exist, this will throw an exception
TEST(ObjectTestMappedFileSource, exception)
{
    kodo::dummy_stack stack;
    kodo::dummy_factory factory;

    factory.m_filename.set_return("file_which_should_not_exist");

    bool exception_caught = false;

    try
    {
        stack.initialize(factory);
    }
    catch (const std::exception& e)
    {
        exception_caught = true;
    }

    EXPECT_TRUE(exception_caught);
}

TEST(ObjectTestMappedFileSource, api)
{
    // Create some files for the test
    auto path_one = "file_which_should_always_exist_one";
    auto path_two = "file_which_should_always_exist_two";

    std::fstream fs_one(path_one, std::fstream::out);
    assert(fs_one.is_open());
    fs_one.close();

    std::fstream fs_two(path_two, std::fstream::out);
    assert(fs_two.is_open());
    fs_two.close();

    uint32_t size_one = 1000;
    uint32_t size_two = 2000;

    boost::filesystem::resize_file(path_one, size_one);
    boost::filesystem::resize_file(path_two, size_two);

    kodo::dummy_stack stack;
    kodo::dummy_factory factory;

    factory.m_filename.set_return(path_one);
    stack.initialize(factory);

    // Comparison function which checks the size of the storage object
    // and that the data pointer is not null
    auto compare = [](const std::tuple<sak::const_storage>& actual,
                      const std::tuple<sak::const_storage>& expected) -> bool
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

    sak::const_storage check_one = {(uint8_t*)0xdeadbeef, size_one};
    EXPECT_TRUE((bool) factory.m_set_storage.expect_calls(compare)
                    .with(check_one));

    // Initialize again
    factory.m_filename.set_return(path_two);
    stack.initialize(factory);

    sak::mutable_storage check_two = {(uint8_t*)0xdeadbeef, size_two};
    EXPECT_TRUE((bool) factory.m_set_storage.expect_calls(compare)
                    .with(check_one)
                    .with(check_two));

}
