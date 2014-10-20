// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <gtest/gtest.h>

#include <stub/call.hpp>

#include <kodo/rlnc/fulcrum_info.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
	namespace
	{

		class dummy_layer
		{
		public:

			class factory_base
			{
			public:

			    factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
			    {
                    (void) max_symbols;
                    (void) max_symbol_size;
			    }

			public:

				uint32_t symbols()
				{
					return m_symbols();
				}


			public:

				stub::call<uint32_t()> m_symbols;
			};

		public:

			template<class Factory>
			void initialize(Factory& the_factory)
			{
                (void) the_factory;
            }
		};

	}

    template<uint32_t MaxExpansion, uint32_t DefaultExpansion>
	class dummy_stack
		: public fulcrum_info<MaxExpansion, DefaultExpansion, dummy_layer>
	{ };
}

/// Test that the API returns the right values
TEST(TestFulcrumInfo, api)
{
	const uint32_t MaxExpansion = 8U;
	const uint32_t DefaultExpansion = 4U;

    uint32_t symbols = 10U;
    uint32_t symbol_size = 10U;

    using stack_type = kodo::dummy_stack<MaxExpansion, DefaultExpansion>;

    // Check the factory
    stack_type::factory_base factory(symbols, symbol_size);

    EXPECT_EQ(factory.max_expansion(), MaxExpansion);
    EXPECT_EQ(factory.expansion(), DefaultExpansion);
    EXPECT_EQ(factory.max_inner_symbols(), MaxExpansion + symbols);

    factory.set_expansion(6U);

    EXPECT_EQ(factory.expansion(), 6U);
    EXPECT_EQ(factory.max_expansion(), MaxExpansion);
    EXPECT_EQ(factory.max_inner_symbols(), MaxExpansion + symbols);

    // Check the stack
    stack_type stack;
    stack.initialize(factory);


    //Test of the factory funtion explicit by testing the value of m_expansion
	EXPECT_EQ(factory.expansion(), DefaultExpansion);


    //Test of the initialize function
	factory.m_symbols.set_return(1U);
	dummy.initialize(factory);

	EXPECT_EQ(dummy.m_initialize.calls(), 1);
	EXPECT_EQ(factory.m_symbols.calls(), 1);

	dummy.initialize(factory);
	EXPECT_EQ(dummy.m_initialize.calls(), 2);
	EXPECT_EQ(factory.m_symbols.calls(), 2);
}
