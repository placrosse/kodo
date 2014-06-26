// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_payload_precoder.cpp Unit tests for the
///       kodo::payload_codec layer

#include <cstdint>
#include <gtest/gtest.h>

#include <stub/call.hpp>

#include <kodo/payload_precoder.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
		
	    //Dummy API for the payload_precoder class	
		class dummy_layer
		{
		public:
			template<class Factory>
			void initialize(Factory& the_factory)
			{	
			}

			void precode()
			{
				m_precode();
			}

			uint32_t encode(uint8_t* payload)
			{
				return m_encode(payload);
			}

			stub::call<void()> m_precode;
			stub::call<uint32_t (uint8_t*)> m_encode;			
		};

		class dummy_factory
		{
		};

		// Instantiate a dummy_class with the payload_precoder class
		// Contain method to get protected m_precoding_complete member
		class dummy_class
			: public payload_precoder<dummy_layer>
		{
		public:
			bool get_m_precoding_complete()
			{
				return m_precoding_complete;
			}
		};
    }
}

//Testrun
TEST(TestPayloadPrecoder, api)
{
    //Testing value of m_precoding_complete with three asserts
	kodo::dummy_class dummy;
	kodo::dummy_factory factory;
	
	///Set up for first assert for m_precoding_complete
	dummy.m_encode.set_return(2U);
	dummy.initialize(factory);
	
	EXPECT_FALSE(dummy.get_m_precoding_complete());

	///Set up second assert for m_precoding_complete
	uint32_t dummy_value;
	uint8_t* dummy_pointer; 
	uint8_t pointee = 6U;
	dummy_pointer = &pointee; // Must be any other value than NULL
	dummy_value = dummy.encode(dummy_pointer);
	
	EXPECT_TRUE(dummy.get_m_precoding_complete());

	///Third assert for m_precoding_complete
	dummy.initialize(factory);
	
	EXPECT_FALSE(dummy.get_m_precoding_complete());

	///Testing the number og times precode and encode are called
	///Encode called to times to test its if statement
	dummy.encode(dummy_pointer);
	dummy.encode(dummy_pointer);

	EXPECT_EQ(dummy.m_precode.calls(), 2U);
	EXPECT_EQ(dummy.m_encode.calls(), 3U);
}
