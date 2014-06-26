// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_rebind_factory.cpp Unit tests for the rebind_factory helper

#include <cstdint>

#include <gtest/gtest.h>

#include <kodo/rebind_factory.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        template<class T>
        class factory_test
        {
        public:
            using type = T;

            template<class U>
            using other = factory_test<U>;
        };

        class dummy
        {
        public:
            using factory = factory_test<uint8_t>;
        };
    }
}



TEST(TestRebindFactory, api)
{
    using type = kodo::rebind_factory<kodo::dummy, uint32_t>;
    bool result = std::is_same<type, kodo::factory_test<uint32_t>>::value;
    EXPECT_TRUE(result);
}
