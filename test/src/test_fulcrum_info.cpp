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
			class factory
			{
			public:
				uint32_t symbols()
				{
					return m_symbols();
				}
				
			public:		
			    factory(uint32_t max_symbols, uint32_t max_symbol_size)
			    {
				    m_factory();
			    }
			
			public:
				stub::call<uint32_t()> m_symbols;
				stub::call<void()> m_factory;
			};
	
		public:
			
			template<class Factory>
			void initialize(Factory& the_factory)
			{
				m_initialize();
			}		
			
		public:
			stub::call<void()> m_initialize;
		};

	}
	
    template
    <
	    uint32_t MaxExpansion,
	    uint32_t DefaultExpansion
	>
	
	class dummy_class
		: public fulcrum_info<MaxExpansion, DefaultExpansion, dummy_layer>
	{
	};
}


TEST(TestFulcrumInfo, api)
{

	const uint32_t MaxExpansion = 8U;
	const uint32_t DefaultExpansion =4U;

	kodo::dummy_class <MaxExpansion, DefaultExpansion>dummy;
	kodo::dummy_class <MaxExpansion, DefaultExpansion>::factory factory(10, 10);

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
