// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>
#include <kodo/seed_generator.hpp>
#include <stub/call.hpp>


namespace
{
    class final
    {
    public:
        typedef uint32_t seed_type;

        class factory_base
        {
        public:
            factory_base(uint32_t, uint32_t)
            { }
        };

        template<class Factory>
        void initialize(Factory&)
        {
        }

        void seed(seed_type seed)
        {
            m_seed_function(seed);
        }

        stub::call<void(seed_type)> m_seed_function;
    };

    class dummy : public kodo::seed_generator<final>
    {
    public:
        using factory = dummy::factory_base;
    };
}

TEST(TestSeedGenerator, test_seed_generator)
{
    uint32_t seed = 42;

    dummy::factory factory(0, 0);
    factory.set_seed(seed);
    EXPECT_EQ(seed, factory.seed());
    dummy stack;

    stack.initialize(factory);
    stack.initialize(factory);

    EXPECT_EQ(2, stack.m_seed_function.calls());
    EXPECT_TRUE((bool)stack.m_seed_function.expect_calls()
                           .with(seed)
                           .with(seed));
}
