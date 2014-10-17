// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <gtest/gtest.h>
#include <sak/storage.hpp>

#include <stub/call.hpp>

#include <kodo/rlnc/fulcrum_expansion_storage.hpp>

namespace kodo
{

    /// @todo copyright year

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // The dummy nested class is used to represent the inner
        // code. The inner code is initialized by the
        // fulcrum_expansion_storage layer using the set_symbols()
        // function.
		class dummy_nested
		{
		public:
			void set_symbol(uint32_t index,
                            const sak::const_storage& symbol_storage)
			{
				m_set_symbol(index, symbol_storage);
			}

			uint32_t symbol_size() const
			{
				return m_symbol_size();
			}

			uint32_t symbols() const
			{
				return m_symbols();
			}

		public:
			stub::call<void(uint32_t,const sak::const_storage&)> m_set_symbol;
			stub::call<uint32_t()> m_symbol_size;
			stub::call<uint32_t()> m_symbols;

		};

	    // Dummy SuperCoder layer for the fulcrum_expansion_storage layer
        class dummy_layer
		{
		public:

			using nested_stack_type = dummy_nested;

		public:

			template<class Factory>
			void construct(Factory& the_factory)
			{
				m_construct();
			}

			template<class Factory>
			void initialize(Factory& the_factory)
			{
				m_initialize();
			}

			nested_stack_type* nested()
			{
				return &m_nested;
			}

			uint32_t inner_symbols() const
			{
				return m_inner_symbols();
			}

			uint32_t symbol_size() const
			{
				return m_symbol_size();
			}

			uint32_t symbols() const
			{
				return m_symbols();
			}

			uint8_t* symbol(uint32_t index)
			{
				return m_symbol();
			}

			uint32_t expansion() const
			{
				return m_expansion();
			}

		public:

			nested_stack_type m_nested;

		public:
			stub::call<void()> m_construct;
			stub::call<void()> m_initialize;
			stub::call<void()> m_set_symbols;
			stub::call<uint32_t()> m_inner_symbols;
			stub::call<uint32_t()> m_symbol_size;
			stub::call<uint32_t()> m_symbols;
			stub::call<uint8_t*()> m_symbol;
			stub::call<uint32_t()> m_expansion;
		};

        // Dummy factory used to initialize the stack
		class dummy_factory
		{
		public:

			uint32_t max_expansion()
			{
				return m_max_expansion();
			}

			uint32_t max_symbol_size()
			{
				return m_max_symbol_size();
			}

		public:

			stub::call<uint32_t()> m_max_expansion;
			stub::call<uint32_t()> m_max_symbol_size;
		};
	}

    /// The fulcrum_expansion_storage layer only works with shallow
    /// storage layers, this is checked at compile time. To make sure
    /// compiles we have to mark our dummy_nested stack as shallow
	template<>
    struct has_shallow_symbol_storage<dummy_nested>
    {
        static const bool value = true;

    };

    /// The test stack
	class dummy_stack
	    : public fulcrum_expansion_storage<dummy_layer>
	{ };
}

/// Testrun
TEST(TestFulcrumExpansionStorage, api)
{



	//Several tests designed to check the number of iterations in the
	//construct function. Every time a new dummy_class is initialized
	//to reset the stubs.

	//Set up of first test of construct function
	kodo::dummy_class dummy_1;
	kodo::dummy_factory factory_1;
	factory_1.m_max_expansion.set_return(0U);
	factory_1.m_max_symbol_size.set_return(32U);
	dummy_1.construct(factory_1);

	EXPECT_EQ(dummy_1.m_construct.calls(), 1U);
	EXPECT_EQ(factory_1.m_max_expansion.calls(), 1U);
	EXPECT_EQ(factory_1.m_max_symbol_size.calls(), 0U);

	//Set up of second test of construct function
	kodo::dummy_class dummy_2;
	kodo::dummy_factory factory_2;
	factory_2.m_max_expansion.set_return(32U);
	factory_2.m_max_symbol_size.set_return(32U);
	dummy_2.construct(factory_2);

	EXPECT_EQ(dummy_2.m_construct.calls(), 1U);
	EXPECT_EQ(factory_2.m_max_expansion.calls(), 1U);
	EXPECT_EQ(factory_2.m_max_symbol_size.calls(), 32U);

	//Set up of third test of construct function
	kodo::dummy_class dummy_3;
	kodo::dummy_factory factory_3;
	factory_3.m_max_expansion.set_return(16U);
	factory_3.m_max_symbol_size.set_return(32U);
	dummy_3.construct(factory_3);

	EXPECT_EQ(dummy_3.m_construct.calls(), 1U);
	EXPECT_EQ(factory_3.m_max_expansion.calls(), 1U);
	EXPECT_EQ(factory_3.m_max_symbol_size.calls(), 16U);
}

TEST(TestFulcrumExpansionStorage, api2)
{
	//Several tests designed to check the number of iteration
	//in the initialize function.

	//First test of first for-loop. m_expansions set to zero to make sure
	//that the second loop isn't entered.
	kodo::dummy_class dummy_4;
	kodo::dummy_factory factory_4;
	dummy_4.m_nested.m_symbols.set_return(8U);
	dummy_4.m_inner_symbols.set_return(8U);
	dummy_4.m_nested.m_symbol_size.set_return(32U);
	dummy_4.m_symbol_size.set_return(32U);

	dummy_4.m_symbols.set_return(8U);
	dummy_4.m_expansion.set_return(0U);

	uint8_t dummy_value_1 = 10;
	dummy_4.m_symbol.set_return(&dummy_value_1);

	dummy_4.initialize(factory_4);
	EXPECT_EQ(dummy_4.m_nested.m_set_symbol.calls(), 8U);

}

TEST(TestFulcrumExpansionStorage, api3)
{
	//Second test of first for-loop. m_expansions set to zero to make sure
	//that the second loop isn't entered.
	kodo::dummy_class dummy_5;
	kodo::dummy_factory factory_5;
	dummy_5.m_nested.m_symbols.set_return(8U);
	dummy_5.m_inner_symbols.set_return(8U);
	dummy_5.m_nested.m_symbol_size.set_return(32U);
	dummy_5.m_symbol_size.set_return(32U);

	dummy_5.m_symbols.set_return(1U);
	dummy_5.m_expansion.set_return(0U);

	uint8_t dummy_value_2 = 10;
	dummy_5.m_symbol.set_return(&dummy_value_2);

	dummy_5.initialize(factory_5);
	EXPECT_EQ(dummy_5.m_nested.m_set_symbol.calls(), 1U);
}

TEST(TestFulcrumExpansionStorage, api4)
{

	//First test of second for-loop. m_symbols set to one to make sure
	//that the first loop is entered only once.
	kodo::dummy_class dummy_6;
	kodo::dummy_factory factory_6;
	dummy_6.m_nested.m_symbols.set_return(8U);
	dummy_6.m_inner_symbols.set_return(8U);
	dummy_6.m_nested.m_symbol_size.set_return(32U);
	dummy_6.m_symbol_size.set_return(32U);

	dummy_6.m_symbols.set_return(1U);
	dummy_6.m_expansion.set_return(1U);
	factory_6.m_max_expansion.set_return(1U);
	factory_6.m_max_symbol_size.set_return(2U);

	uint8_t dummy_value_3 = 10;
	dummy_6.m_symbol.set_return(&dummy_value_3);

	dummy_6.construct(factory_6);
	dummy_6.initialize(factory_6);
	//Expected is one higher than the return value of m_expansion
	//since the first loop is entered once.
	EXPECT_EQ(dummy_6.m_nested.m_set_symbol.calls(), 2U);
}

TEST(TestFulcrumExpansionStorage, api5)
{
	//Second test of second for-loop. m_symbols set to one to make sure
	//that the first loop is entered only once.
	kodo::dummy_class dummy_7;
	kodo::dummy_factory factory_7;
	dummy_7.m_nested.m_symbols.set_return(8U);
	dummy_7.m_inner_symbols.set_return(8U);
	dummy_7.m_nested.m_symbol_size.set_return(32U);
	dummy_7.m_symbol_size.set_return(32U);

	dummy_7.m_symbols.set_return(1U);
	dummy_7.m_expansion.set_return(4U);
	factory_7.m_max_expansion.set_return(16U);
	factory_7.m_max_symbol_size.set_return(32U);

	uint8_t dummy_value_4 = 10;
	dummy_7.m_symbol.set_return(&dummy_value_4);

	dummy_7.construct(factory_7);
	dummy_7.initialize(factory_7);
	//Expected is one higher than the return value of m_expansion
	//since the first loop is entered once.
	EXPECT_EQ(dummy_7.m_nested.m_set_symbol.calls(), 5U);

}
