// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_nested_catch_all.cpp Unit tests for the nested catch all

#include <gtest/gtest.h>

#include <kodo/nested_catch_all.hpp>

#include "stub.hpp"

namespace
{

    // This class with "act" as the nested stack in our unit test.
    class dummy_nested
    {
    public:

        // The nested stack must specify the value_type used
        using value_type = uint8_t;

    public:

        // Since all calls forward to the nested factory simply return
        // an uint32_t we just make each of them return a different
        // value in that way we can check that the correct one was
        // called (the value sort of serves as an id).
        class factory
        {
        public:

            uint32_t max_symbols() const
            {
                return m_max_symbols();
            }

            uint32_t max_symbol_size() const
            {
                return m_max_symbol_size();
            }

            uint32_t max_block_size() const
            {
                return m_max_block_size();
            }

            uint32_t max_header_size() const
            {
                return m_max_header_size();
            }

            uint32_t max_id_size() const
            {
                return m_max_id_size();
            }

            uint32_t max_payload_size() const
            {
                return m_max_payload_size();
            }

            uint32_t max_coefficient_vector_size() const
            {
                return m_max_coefficient_vector_size();
            }

            uint32_t symbols() const
            {
                return m_symbols();
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size();
            }

            stub::call<uint32_t ()> m_max_symbols;
            stub::call<uint32_t ()> m_max_symbol_size;
            stub::call<uint32_t ()> m_max_block_size;
            stub::call<uint32_t ()> m_max_header_size;
            stub::call<uint32_t ()> m_max_id_size;
            stub::call<uint32_t ()> m_max_payload_size;
            stub::call<uint32_t ()> m_max_coefficient_vector_size;
            stub::call<uint32_t ()> m_symbols;
            stub::call<uint32_t ()> m_symbol_size;
        };

    public:

        //------------------------------------------------------------------
        // SYMBOL STORAGE API
        //------------------------------------------------------------------

        void copy_symbols(const sak::mutable_storage& dest) const
        {
            m_copy_symbols(dest);
        }

        void copy_symbol(uint32_t index, const sak::mutable_storage &dest) const
        {
            m_copy_symbol(index, dest);
        }

        uint8_t* symbol(uint32_t index)
        {
            return m_symbol(index);
        }

        const uint8_t* symbol(uint32_t index) const
        {
            return m_symbol_const(index);
        }

        value_type* symbol_value(uint32_t index)
        {
            return m_symbol_value(index);
        }

        const value_type* symbol_value(uint32_t index) const
        {
            return m_symbol_value_const(index);
        }

        uint32_t symbols() const
        {
            return m_symbols();
        }

        uint32_t symbol_size() const
        {
            return m_symbol_size();
        }

        uint32_t symbol_length() const
        {
            return m_symbol_length();
        }

        uint32_t block_size() const
        {
            return m_block_size();
        }

        uint32_t symbols_available() const
        {
            return m_symbols_available();
        }

        uint32_t symbols_initialized() const
        {
            return m_symbols_initialized();
        }

        bool is_symbols_available() const
        {
            return m_is_symbols_available();
        }

        bool is_symbols_initialized() const
        {
            return m_is_symbols_initialized();
        }

        bool is_symbol_available(uint32_t symbol_index) const
        {
            return m_is_symbol_available(symbol_index);
        }

        bool is_symbol_initialized(uint32_t symbol_index) const
        {
            return m_is_symbol_initialized(symbol_index);
        }

        //------------------------------------------------------------------
        // COEFFICIENT STORAGE API
        //------------------------------------------------------------------

        uint32_t coefficient_vector_size() const
        {
            return m_coefficient_vector_size();
        }

        uint32_t coefficient_vector_length() const
        {
            return m_coefficient_vector_length();
        }

        value_type* coefficient_vector_values(uint32_t index)
        {
            return m_coefficient_vector_values(index);
        }

        const value_type* coefficient_vector_values(uint32_t index) const
        {
            return m_coefficient_vector_values_const(index);
        }

        uint8_t* coefficient_vector_data(uint32_t index)
        {
            return m_coefficient_vector_data(index);
        }

        const uint8_t* coefficient_vector_data(uint32_t index) const
        {
            return m_coefficient_vector_data_const(index);
        }

        //------------------------------------------------------------------
        // FINITE FIELD API
        //------------------------------------------------------------------

        void multiply(value_type* symbol_dest, value_type coefficient,
                      uint32_t symbol_length)
        {
            m_multiply(symbol_dest, coefficient, symbol_length);
        }

        void multiply_add(value_type* symbol_dest, const value_type* symbol_src,
                          value_type coefficient, uint32_t symbol_length)
        {
            m_multiply_add(symbol_dest, symbol_src, coefficient, symbol_length);
        }

        void add(value_type* symbol_dest, const value_type* symbol_src,
                 uint32_t symbol_length)
        {
            m_add(symbol_dest, symbol_src, symbol_length);
        }

        void multiply_subtract(value_type* symbol_dest, const value_type* symbol_src,
                               value_type coefficient, uint32_t symbol_length)
        {
            m_multiply_substract(symbol_dest, symbol_src,
                                 coefficient, symbol_length);
        }

        void subtract(value_type* symbol_dest, const value_type* symbol_src,
                      uint32_t symbol_length)
        {
            m_substract(symbol_dest, symbol_src, symbol_length);
        }

        value_type invert(value_type value)
        {
            return m_invert(value);
        }

        //------------------------------------------------------------------
        // CODEC API
        //------------------------------------------------------------------

        void encode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            m_encode_symbol(symbol_data, coefficients);
        }

        void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            m_encode_symbol_index(symbol_data, symbol_index);
        }

        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            m_decode_symbol(symbol_data, coefficients);
        }

        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            m_decode_symbol_index(symbol_data, symbol_index);
        }

        bool is_complete() const
        {
            return m_is_complete();
        }

        uint32_t rank() const
        {
            return m_rank();
        }

        bool is_symbol_pivot(uint32_t index) const
        {
            return m_is_symbol_pivot(index);
        }


        //------------------------------------------------------------------
        // SYMBOL STORAGE API
        //------------------------------------------------------------------

        stub::call<void (const sak::mutable_storage&)> m_copy_symbols;
        stub::call<void (uint32_t,const sak::mutable_storage&)> m_copy_symbol;
        stub::call<uint8_t*(uint32_t)> m_symbol;
        stub::call<const uint8_t*(uint32_t)> m_symbol_const;
        stub::call<value_type*(uint32_t)> m_symbol_value;
        stub::call<const value_type*(uint32_t)> m_symbol_value_const;
        stub::call<uint32_t ()> m_symbols;
        stub::call<uint32_t ()> m_symbol_size;
        stub::call<uint32_t ()> m_symbol_length;
        stub::call<uint32_t ()> m_block_size;
        stub::call<uint32_t ()> m_symbols_available;
        stub::call<uint32_t ()> m_symbols_initialized;
        stub::call<bool ()> m_is_symbols_available;
        stub::call<bool ()> m_is_symbols_initialized;
        stub::call<bool (uint32_t)> m_is_symbol_available;
        stub::call<bool (uint32_t)> m_is_symbol_initialized;

        //------------------------------------------------------------------
        // COEFFICIENT STORAGE API
        //------------------------------------------------------------------

        /// @todo When writing these functions I noticed that they are
        ///       not very consistent with the symbol storage
        ///       API. I.e. here we have coefficient_vector_values
        ///       whereas for symbols we have symbol_value

        stub::call<uint32_t ()> m_coefficient_vector_size;
        stub::call<uint32_t ()> m_coefficient_vector_length;
        stub::call<value_type* (uint32_t)> m_coefficient_vector_values;
        stub::call<const value_type* (uint32_t)> m_coefficient_vector_values_const;
        stub::call<uint8_t* (uint32_t)> m_coefficient_vector_data;
        stub::call<const uint8_t* (uint32_t)> m_coefficient_vector_data_const;

        //------------------------------------------------------------------
        // FINITE FIELD API
        //------------------------------------------------------------------

        stub::call<void (value_type*, value_type, uint32_t)>
            m_multiply;

        stub::call<void (value_type*, const value_type*, value_type, uint32_t)>
            m_multiply_add;

        stub::call<void (value_type*, const value_type*, uint32_t)>
            m_add;

        stub::call<void (value_type*, const value_type*, value_type, uint32_t)>
            m_multiply_substract;

        stub::call<void (value_type*, const value_type*, uint32_t)>
            m_substract;

        stub::call<value_type (value_type)>
            m_invert;

        //------------------------------------------------------------------
        // CODEC API
        //------------------------------------------------------------------

        stub::call<void (uint8_t*, uint8_t*)> m_encode_symbol;
        stub::call<void (uint8_t*, uint32_t)> m_encode_symbol_index;
        stub::call<void (uint8_t*, uint8_t*)> m_decode_symbol;
        stub::call<void (uint8_t*, uint32_t)> m_decode_symbol_index;
        stub::call<bool ()> m_is_complete;
        stub::call<uint32_t ()> m_rank;

        /// @todo The is_pivot should be move to the symbol storage API
        stub::call<bool (uint32_t)> m_is_symbol_pivot;

    };

    // This layer will provide all the functions invoked by the
    // nested_catch_all layer so that we can check that all calls are
    // correctly forwarded.
    class dummy_layer
    {
    public:

        // The nested_catch_all layer requires that the nested stack
        // is typified
        using nested_stack_type = dummy_nested;

    public:

        class factory
        {
        public:

            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : m_max_symbols(max_symbols),
                  m_max_symbol_size(max_symbol_size)
            { }

            const nested_stack_type::factory& nested() const
            {
                return m_nested;
            }

        public:

            uint32_t m_max_symbols;
            uint32_t m_max_symbol_size;

            nested_stack_type::factory m_nested;

        };

    public: // The nested stack accesses

        nested_stack_type* nested()
        {
            return &m_nested;
        }

        const nested_stack_type* nested() const
        {
            return &m_nested;
        }

    public:

        nested_stack_type m_nested;

    };

    // Dummy stack for testing the nested_catch_all layer
    using dummy_stack = kodo::nested_catch_all<dummy_layer>;

}

    /// Run the tests typical coefficients stack
TEST(TestNestedCatchAll, api)
{
    uint32_t max_symbols = 1U;
    uint32_t max_symbol_size = 10U;

    // Test the factory
    {
        dummy_stack::factory factory(max_symbols, max_symbol_size);
        EXPECT_EQ(factory.m_max_symbols, max_symbols);
        EXPECT_EQ(factory.m_max_symbol_size, max_symbol_size);

        auto& nested = factory.m_nested;

        nested.m_max_symbols.set_return(1U);
        EXPECT_EQ(factory.max_symbols(), 1U);
        EXPECT_TRUE(nested.m_max_symbols.called_once_with());

        nested.m_max_symbol_size.set_return(2U);
        EXPECT_EQ(factory.max_symbol_size(), 2U);
        EXPECT_TRUE(nested.m_max_symbol_size.called_once_with());

        nested.m_max_block_size.set_return(3U);
        EXPECT_EQ(factory.max_block_size(), 3U);
        EXPECT_TRUE(nested.m_max_block_size.called_once_with());

        nested.m_max_header_size.set_return(4U);
        EXPECT_EQ(factory.max_header_size(), 4U);
        EXPECT_TRUE(nested.m_max_header_size.called_once_with());

        nested.m_max_id_size.set_return(5U);
        EXPECT_EQ(factory.max_id_size(), 5U);
        EXPECT_TRUE(nested.m_max_id_size.called_once_with());

        nested.m_max_payload_size.set_return(6U);
        EXPECT_EQ(factory.max_payload_size(), 6U);
        EXPECT_TRUE(nested.m_max_payload_size.called_once_with());

        nested.m_max_coefficient_vector_size.set_return(7U);
        EXPECT_EQ(factory.max_coefficient_vector_size(), 7U);
        EXPECT_TRUE(nested.m_max_coefficient_vector_size.called_once_with());

        nested.m_symbols.set_return(8U);
        EXPECT_EQ(factory.symbols(), 8U);
        EXPECT_TRUE(nested.m_symbols.called_once_with());

        nested.m_symbol_size.set_return(9U);
        EXPECT_EQ(factory.symbol_size(), 9U);
        EXPECT_TRUE(nested.m_symbol_size.called_once_with());
    }

    // Test the stack
    {
        dummy_stack stack;

        auto& nested = stack.m_nested;

        std::vector<uint8_t> data(200, 'z');
        std::vector<uint8_t> data1(200, 'z');

        auto compare_stoarge = [](std::tuple<sak::mutable_storage> a,
                                  std::tuple<sak::mutable_storage> b)
        {
            auto aa = std::get<0>(a);
            auto bb = std::get<0>(b);
            if(aa.m_data != bb.m_data)
                return false;
            if(aa.m_size != bb.m_size)
                return false;
            return true;
        };

        auto check_copy_symbol = [](std::tuple<uint32_t, sak::mutable_storage> a,
                                  std::tuple<uint32_t, sak::mutable_storage> b)
        {
            if(std::get<0>(a) != std::get<0>(b))
            {
                return false;
            }

            auto aa = std::get<1>(a);
            auto bb = std::get<1>(b);
            if(aa.m_data != bb.m_data)
                return false;
            if(aa.m_size != bb.m_size)
                return false;
            return true;
        };


        stack.copy_symbols(sak::storage(data));
        EXPECT_TRUE(nested.m_copy_symbols.called_once_with(
                        sak::storage(data), compare_stoarge));

        stack.copy_symbol(10U, sak::storage(data));
        EXPECT_TRUE(nested.m_copy_symbol.called_once_with(
                        10U, sak::storage(data), check_copy_symbol));

    }


    // expected_factory_calls.m_max_symbols.add_call().set_return(r);
    // EXPECT_EQ(expected_factory_calls, );




    // call<uint32_t()> reference_call;
    // call<uint32_t()> expected_call;

    // expected_call.add_call().set_return(1U);
    // reference_call.set_return(

    // expected_call.set_return(1U);

    // EXPECT_EQ(factory.max_symbols(), 1U);
    // }

    // call<uint32_t()> call_max_symbols;
    // call_max_symbols.add_call().set_return(1U);
    // EXPECT_EQ(nested_factory.m_call_max_symbols, call_max_symbols);
    //




}
