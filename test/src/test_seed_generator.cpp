// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>
#include <kodo/seed_generator.hpp>
#include <stub/call.hpp>


namespace kodo
{
    class dummy_layer
    {
    public:
        typedef uint32_t seed_type;

        class factory_base
        {
        public:
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
            {
                (void)max_symbols;
                (void)max_symbol_size;
            }
        };

        template<class Factory>
        void initialize(Factory& factory)
        {
            (void)factory;
        }

        void seed(seed_type seed)
        {
            m_seed_function(seed);
        }

        stub::call<void(seed_type)> m_seed_function;
    };

    class dummy_stack : public seed_generator<dummy_layer>
    {
    public:
        using factory = dummy_stack::factory_base;
    };
}

TEST(TestSeedGenerator, test_seed_generator)
{
    uint32_t seed = 42;

    kodo::dummy_stack::factory factory(0, 0);
    factory.set_seed(seed);
    EXPECT_EQ(seed, factory.seed());
    kodo::dummy_stack stack;

    stack.initialize(factory);
    stack.initialize(factory);

    EXPECT_TRUE((bool)stack.m_seed_function.expect_calls()
                           .with(seed)
                           .with(seed));
}
