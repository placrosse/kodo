// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_is_complete_decoder.cpp Unit tests for the
///       is_complete_decoder class

#include <cstdint>

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/object/is_complete_decoder.hpp>

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

        uint32_t block_size() const
        {
            return m_block_size();
        }

        stub::call<void()> m_initialize;
        stub::call<uint32_t()> m_block_size;
    };

    // Helper stack
    class dummy_stack : public object::is_complete_decoder<dummy_layer>
    { };

    // Helper factory
    class dummy_factory
    { };
}
}

/// @todo figure out is this is the right naming
TEST(ObjectTestIsCompleteDecoder, api)
{
    assert(0);
}
