// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_is_complete_callback_decoder.cpp Unit tests for the
///       is_complete_callback_decoder class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/is_complete_callback_decoder.hpp>
#include <kodo/wrap_is_complete_callback_decoder.hpp>
#include <kodo/basic_factory.hpp>

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
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
                m_initialize();
            }

            bool is_complete() const
            {
                return m_is_complete();
            }

            void decode(uint8_t* payload)
            {
                m_decode(payload);
            }

            stub::call<void()> m_initialize;
            stub::call<bool()> m_is_complete;
            stub::call<void(uint8_t*)> m_decode;
        };

        // Helper stack
        class dummy_stack : public is_complete_callback_decoder<dummy_layer>
        { };

        // Helper factory
        class dummy_factory
        { };
    }
}

/// Tests that the code works when the user hasn't specified a callback
TEST(TestIsCompleteCallback, no_callback)
{
    kodo::dummy_factory factory;
    kodo::dummy_stack stack;

    stack.m_is_complete.set_return(false);

    uint8_t* pointer = (uint8_t*)0xdeadbeef;

    stack.initialize(factory);
    EXPECT_TRUE((bool) stack.m_initialize.expect_calls().with());

    stack.decode(pointer);
    EXPECT_TRUE((bool) stack.m_is_complete.expect_calls().with());
    EXPECT_TRUE((bool) stack.m_decode.expect_calls().with(pointer));

    // Trigger that we are complete
    stack.m_is_complete.set_return(true);

    stack.decode(pointer);
    EXPECT_TRUE((bool) stack.m_is_complete.expect_calls()
                    .with()
                    .with());

    EXPECT_TRUE((bool) stack.m_decode.expect_calls()
                    .with(pointer)
                    .with(pointer));
}


/// Tests that the code works when the user has specified a callback
TEST(TestIsCompleteCallback, callback)
{
    kodo::dummy_factory factory;
    kodo::dummy_stack stack;

    stack.m_is_complete.set_return(false);

    uint8_t* pointer = (uint8_t*)0xdeadbeef;

    stub::call<void()> callback;

    stack.initialize(factory);
    stack.set_is_complete_callback(std::bind(std::ref(callback)));

    EXPECT_TRUE((bool) stack.m_initialize.expect_calls()
                    .with());

    EXPECT_TRUE((bool) stack.m_is_complete.expect_calls()
                    .with());

    stack.decode(pointer);

    EXPECT_TRUE((bool) stack.m_is_complete.expect_calls()
                    .with()
                    .repeat(1));

    EXPECT_TRUE((bool) stack.m_decode.expect_calls()
                    .with(pointer));

    // Trigger that we are complete
    stack.m_is_complete.set_return(true);

    stack.decode(pointer);

    EXPECT_TRUE((bool) stack.m_is_complete.expect_calls()
                    .with()
                    .repeat(2));

    EXPECT_TRUE((bool) stack.m_decode.expect_calls()
                    .with(pointer)
                    .repeat(1));

    // Now we should have gotten a callback
    EXPECT_TRUE((bool)callback.expect_calls()
                    .with());

    stack.decode(pointer);

    EXPECT_TRUE((bool) stack.m_is_complete.expect_calls()
                    .with()
                    .repeat(3));

    EXPECT_TRUE((bool) stack.m_decode.expect_calls()
                    .with(pointer)
                    .repeat(2));

    // We check that we still only have the one callback
    EXPECT_TRUE((bool)callback.expect_calls()
                    .with());

}

/// Test that we can wrap an stack with the
/// wrap_is_complete_callback_decoder layer we won't actually do
/// anything but check that it compiles, because the
/// is_complete_callback_decoder is already tested in the above unit
/// tests
namespace kodo
{

// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    /// Helper layer
    class wrap_dummy_stack : public dummy_layer
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

        using factory = basic_factory<wrap_dummy_stack>;
    };
}
}

TEST(TestIsCompleteCallback, wrap)
{
    using stack =
        kodo::wrap_is_complete_callback_decoder<kodo::wrap_dummy_stack>;

    stack::factory f(10,10);
    auto s = f.build();
    (void) s;
}
