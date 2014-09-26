// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/forward_recode_to_encode.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        struct dummy_layer
        {
            uint32_t encode(uint8_t* payload)
            {
                m_payload = payload;
                return m_return;
            }

            uint32_t m_return;
            uint8_t* m_payload;
        };

        class dummy_stack : public forward_recode_to_encode<dummy_layer>
        { };

    }
}

TEST(TestForwardRecodeToEncode, api)
{

    kodo::dummy_stack stack;
    stack.m_return = 10U;

    std::vector<uint8_t> d(100);
    uint32_t size = stack.encode(d.data());

    EXPECT_EQ(stack.m_payload, d.data());
    EXPECT_EQ(size, 10U);

}
