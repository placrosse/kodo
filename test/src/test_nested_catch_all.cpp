// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_nested_catch_all.cpp Unit tests for the nested catch all

#include <gtest/gtest.h>

#include <kodo/nested_catch_all.hpp>

#include <stub/call.hpp>

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

        // Test stubs that "record" the function calls
        stub::call<void (const sak::mutable_storage&)> m_copy_symbols;
        stub::call<void (uint32_t,const sak::mutable_storage&)> m_copy_symbol;
        stub::call<uint8_t* (uint32_t)> m_symbol;
        stub::call<const uint8_t* (uint32_t)> m_symbol_const;
        stub::call<value_type* (uint32_t)> m_symbol_value;
        stub::call<const value_type* (uint32_t)> m_symbol_value_const;
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

        /// @todo When writing these functions I noticed that they are
        ///       not very consistent with the symbol storage
        ///       API. I.e. here we have coefficient_vector_values
        ///       whereas for symbols we have symbol_value

        stub::call<uint32_t ()>
            m_coefficient_vector_size;

        stub::call<uint32_t ()>
            m_coefficient_vector_length;

        stub::call<value_type* (uint32_t)>
            m_coefficient_vector_values;

        stub::call<const value_type* (uint32_t)>
            m_coefficient_vector_values_const;

        stub::call<uint8_t* (uint32_t)>
            m_coefficient_vector_data;

        stub::call<const uint8_t* (uint32_t)>
            m_coefficient_vector_data_const;

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

        void multiply_subtract(value_type* symbol_dest,
                               const value_type* symbol_src,
                               value_type coefficient, uint32_t symbol_length)
        {
            m_multiply_subtract(symbol_dest, symbol_src,
                                 coefficient, symbol_length);
        }

        void subtract(value_type* symbol_dest,
                      const value_type* symbol_src,
                      uint32_t symbol_length)
        {
            m_subtract(symbol_dest, symbol_src, symbol_length);
        }

        value_type invert(value_type value)
        {
            return m_invert(value);
        }

        stub::call<void (value_type*, value_type, uint32_t)>
            m_multiply;

        stub::call<void (value_type*, const value_type*, value_type, uint32_t)>
            m_multiply_add;

        stub::call<void (value_type*, const value_type*, uint32_t)>
            m_add;

        stub::call<void (value_type*, const value_type*, value_type, uint32_t)>
            m_multiply_subtract;

        stub::call<void (value_type*, const value_type*, uint32_t)>
            m_subtract;

        stub::call<value_type (value_type)>
            m_invert;

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

        stub::call<void (uint8_t*, uint8_t*)> m_encode_symbol;
        stub::call<void (uint8_t*, uint32_t)> m_encode_symbol_index;
        stub::call<void (uint8_t*, uint8_t*)> m_decode_symbol;
        stub::call<void (uint8_t*, uint32_t)> m_decode_symbol_index;
        stub::call<bool ()> m_is_complete;
        stub::call<uint32_t ()> m_rank;

        /// @todo The is_pivot should be move to the symbol storage API?
        stub::call<bool (uint32_t)> m_is_symbol_pivot;

        //------------------------------------------------------------------
        // COEFFICIENT GENERATOR API
        //------------------------------------------------------------------

        void generate(uint8_t *symbol_coefficients)
        {
            m_generate(symbol_coefficients);
        }

        void generate_partial(uint8_t *symbol_coefficients)
        {
            m_generate_partial(symbol_coefficients);
        }

        void seed(uint64_t seed_value)
        {
            m_seed(seed_value);
        }

        stub::call<void (uint8_t*)> m_generate;
        stub::call<void (uint8_t*)> m_generate_partial;
        stub::call<void (uint64_t)> m_seed;

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
        EXPECT_TRUE(nested.m_max_symbols.expect_calls().with());

        nested.m_max_symbol_size.set_return(2U);
        EXPECT_EQ(factory.max_symbol_size(), 2U);
        EXPECT_TRUE(nested.m_max_symbol_size.expect_calls().with());

        nested.m_max_block_size.set_return(3U);
        EXPECT_EQ(factory.max_block_size(), 3U);
        EXPECT_TRUE(nested.m_max_block_size.expect_calls().with());

        nested.m_max_header_size.set_return(4U);
        EXPECT_EQ(factory.max_header_size(), 4U);
        EXPECT_TRUE(nested.m_max_header_size.expect_calls().with());

        nested.m_max_id_size.set_return(5U);
        EXPECT_EQ(factory.max_id_size(), 5U);
        EXPECT_TRUE(nested.m_max_id_size.expect_calls().with());

        nested.m_max_payload_size.set_return(6U);
        EXPECT_EQ(factory.max_payload_size(), 6U);
        EXPECT_TRUE(nested.m_max_payload_size.expect_calls().with());

        nested.m_max_coefficient_vector_size.set_return(7U);
        EXPECT_EQ(factory.max_coefficient_vector_size(), 7U);
        EXPECT_TRUE(nested.m_max_coefficient_vector_size.expect_calls().with());

        nested.m_symbols.set_return(8U);
        EXPECT_EQ(factory.symbols(), 8U);
        EXPECT_TRUE(nested.m_symbols.expect_calls().with());

        nested.m_symbol_size.set_return(9U);
        EXPECT_EQ(factory.symbol_size(), 9U);
        EXPECT_TRUE(nested.m_symbol_size.expect_calls().with());
    }

    // Test the stack
    {
        dummy_stack stack;
        const dummy_stack& stack_const = stack;

        using value_type = dummy_stack::value_type;

        auto& nested = stack.m_nested;

        std::vector<uint8_t> data(200, 'z');
        std::vector<uint8_t> data1(200, 'z');

        //------------------------------------------------------------------
        // SYMBOL STORAGE API
        //------------------------------------------------------------------

        // There are two ways of comparing storage objects, is_equal
        // (which is used by operator==()) or is_same. The difference
        // is that is_equal checks whether the two storage objects
        // "contain" the same data, whereas is_same checks whether the
        // two storage objects point to the same chunck of memroy. The
        // latter being the check we want to perform here.
        auto compare_storage = [](std::tuple<sak::mutable_storage> a,
                                  std::tuple<sak::mutable_storage> b)
        {
            return sak::is_same(std::get<0>(a),std::get<0>(b));
        };

        auto check_copy_symbol = [](
            std::tuple<uint32_t, sak::mutable_storage> a,
            std::tuple<uint32_t, sak::mutable_storage> b)
        {
            if (std::get<0>(a) != std::get<0>(b))
            {
                return false;
            }
            return sak::is_same(std::get<1>(a),std::get<1>(b));
        };

        stack.copy_symbols(sak::storage(data));
        EXPECT_TRUE((bool) nested.m_copy_symbols.expect_calls(compare_storage)
                        .with(sak::storage(data));

        stack.copy_symbol(10U, sak::storage(data));
        EXPECT_TRUE((bool) nested.m_copy_symbol.expect_calls(check_copy_symbol)
                        .with(10U, sak::storage(data)));

        nested.m_symbol.set_return((uint8_t*)0xdeadbeef);
        EXPECT_EQ(stack.symbol(1U), (uint8_t*)0xdeadbeef);
        EXPECT_TRUE((bool) nested.m_symbol.expect_calls().with(1U));

        nested.m_symbol_const.set_return((const uint8_t*)0xdeadbeef);
        EXPECT_EQ(stack_const.symbol(2U), (const uint8_t*)0xdeadbeef);
        EXPECT_TRUE((bool) nested.m_symbol_const.expect_calls().with(2U));

        nested.m_symbol_value.set_return((value_type*)0xdeadbeef);
        EXPECT_EQ(stack.symbol_value(3U), (value_type*)0xdeadbeef);
        EXPECT_TRUE((bool) nested.m_symbol_value.expect_calls().with(3U));

        nested.m_symbol_value_const.set_return((const value_type*)0xdeadbeef);
        EXPECT_EQ(stack_const.symbol_value(4U), (const value_type*)0xdeadbeef);
        EXPECT_TRUE((bool) nested.m_symbol_value_const.expect_calls().with(4U));

        nested.m_symbols.set_return(8U);
        EXPECT_EQ(stack.symbols(), 8U);
        EXPECT_TRUE((bool) nested.m_symbols.expect_calls().with());

        nested.m_symbol_size.set_return(89U);
        EXPECT_EQ(stack.symbol_size(), 89U);
        EXPECT_TRUE((bool) nested.m_symbol_size.expect_calls().with());

        nested.m_symbol_length.set_return(66U);
        EXPECT_EQ(stack.symbol_length(), 66U);
        EXPECT_TRUE((bool) nested.m_symbol_length.expect_calls().with());

        nested.m_block_size.set_return(11U);
        EXPECT_EQ(stack.block_size(), 11U);
        EXPECT_TRUE((bool) nested.m_block_size.expect_calls().with());

        nested.m_symbols_available.set_return(112U);
        EXPECT_EQ(stack.symbols_available(), 112U);
        EXPECT_TRUE((bool) nested.m_symbols_available.expect_calls().with());

        nested.m_symbols_initialized.set_return(131U);
        EXPECT_EQ(stack.symbols_initialized(), 131U);
        EXPECT_TRUE((bool) nested.m_symbols_initialized.expect_calls().with());

        nested.m_is_symbols_available.set_return(true);
        EXPECT_EQ(stack.is_symbols_available(), true);
        EXPECT_TRUE((bool) nested.m_is_symbols_available.expect_calls().with());

        nested.m_is_symbols_initialized.set_return(false);
        EXPECT_EQ(stack.is_symbols_initialized(), false);
        EXPECT_TRUE((bool) nested.m_is_symbols_initialized.expect_calls()
                        .with());

        nested.m_is_symbol_available.set_return(true);
        EXPECT_EQ(stack.is_symbol_available(42U), true);
        EXPECT_TRUE((bool) nested.m_is_symbol_available.expect_calls()
                        .with(42U));

        nested.m_is_symbol_initialized.set_return(false);
        EXPECT_EQ(stack.is_symbol_initialized(23U), false);
        EXPECT_TRUE((bool) nested.m_is_symbol_initialized.expect_calls()
                        .with(23U));

        //------------------------------------------------------------------
        // COEFFICIENT STORAGE API
        //------------------------------------------------------------------

        nested.m_coefficient_vector_size.set_return(8U);
        EXPECT_EQ(stack.coefficient_vector_size(), 8U);
        EXPECT_TRUE((bool) nested.m_symbols.expect_calls().with());

        nested.m_coefficient_vector_length.set_return(89U);
        EXPECT_EQ(stack.coefficient_vector_length(), 89U);
        EXPECT_TRUE((bool) nested.m_coefficient_vector_length.expect_calls()
                        .with());

        nested.m_coefficient_vector_data.set_return((uint8_t*)0xdeadbeef);
        EXPECT_EQ(stack.coefficient_vector_data(1U), (uint8_t*)0xdeadbeef);
        EXPECT_TRUE((bool) nested.m_coefficient_vector_data.expect_calls()
                        .with(1U));

        nested.m_coefficient_vector_data_const.set_return((uint8_t*)0xdeef);
        EXPECT_EQ(stack_const.coefficient_vector_data(2U), (uint8_t*)0xdeef);
        EXPECT_TRUE((bool) nested.m_coefficient_vector_data_const.expect_calls()
                        .with(2U));

        nested.m_coefficient_vector_values.set_return((value_type*)0xdeadbeef);
        EXPECT_EQ(stack.coefficient_vector_values(3U), (value_type*)0xdeadbeef);
        EXPECT_TRUE((bool) nested.m_coefficient_vector_values.expect_calls()
                        .with(3U));

        nested.m_coefficient_vector_values_const.set_return((value_type*)0xd);
        EXPECT_EQ(stack_const.coefficient_vector_values(4U), (value_type*)0xd);
        EXPECT_TRUE((bool) nested.m_coefficient_vector_values_const
                        .expect_calls().with(4U));

        //------------------------------------------------------------------
        // FINITE FIELD API
        //------------------------------------------------------------------

        stack.multiply((value_type*)0xa, (value_type)1U, 3U);

        EXPECT_TRUE((bool) nested.m_multiply.expect_calls()
                        .with((value_type*)0xa, (value_type)1U, 3U));

        stack.multiply_add((value_type*)0xa, (value_type*)0xb,
                           (value_type)1U, 3U);

        EXPECT_TRUE((bool) nested.m_multiply_add.expect_calls().with(
                        (value_type*)0xa, (value_type*)0xb,
                        (value_type)1U, 3U));

        stack.add((value_type*)0xa, (value_type*)0xc, 3U);

        EXPECT_TRUE((bool) nested.m_add.expect_calls().with(
                        (value_type*)0xa, (value_type*)0xc, 3U));

        stack.multiply_subtract((value_type*)0xa, (value_type*)0xb,
                                (value_type)1U, 3U);

        EXPECT_TRUE((bool) nested.m_multiply_subtract.expect_calls().with(
                        (value_type*)0xa, (value_type*)0xb,
                        (value_type)1U, 3U));

        stack.subtract((value_type*)0xa, (value_type*)0xc, 3U);
        EXPECT_TRUE((bool) nested.m_subtract.expect_calls().with(
                        (value_type*)0xa, (value_type*)0xc, 3U));

        //------------------------------------------------------------------
        // CODEC API
        //------------------------------------------------------------------

        stack.encode_symbol((uint8_t*)0xa, (uint8_t*)0xb);
        EXPECT_TRUE((bool) nested.m_encode_symbol.expect_calls().with(
                        (uint8_t*)0xa, (uint8_t*)0xb));

        stack.encode_symbol((uint8_t*)0xa, 50);
        EXPECT_TRUE((bool) nested.m_encode_symbol_index.expect_calls().with(
                        (uint8_t*)0xa, 50));

        stack.decode_symbol((uint8_t*)0xc, (uint8_t*)0xd);
        EXPECT_TRUE((bool) nested.m_decode_symbol.expect_calls().with(
                        (uint8_t*)0xc, (uint8_t*)0xd));

        stack.decode_symbol((uint8_t*)0xa, 50);
        EXPECT_TRUE((bool) nested.m_decode_symbol_index.expect_calls().with(
                        (uint8_t*)0xa, 50));

        nested.m_is_complete.set_return(true);
        EXPECT_TRUE(stack.is_complete());
        EXPECT_TRUE((bool) nested.m_is_complete.expect_calls().with());

        nested.m_rank.set_return(42);
        EXPECT_EQ(stack.rank(), 42);
        EXPECT_TRUE((bool) nested.m_rank.expect_calls().with());

        nested.m_is_symbol_pivot.set_return({true,false});
        EXPECT_TRUE(stack.is_symbol_pivot(15));
        EXPECT_TRUE((bool) nested.m_is_symbol_pivot.called_with(15));
        EXPECT_FALSE(stack.is_symbol_pivot(12));
        EXPECT_TRUE((bool) nested.m_is_symbol_pivot.called_with(12));

        //------------------------------------------------------------------
        // COEFFICIENT GENERATOR API
        //------------------------------------------------------------------

        stack.generate((uint8_t*)0x3);
        EXPECT_TRUE(nested.m_generate.expect_calls().with((uint8_t*)0x3));

        stack.generate_partial((uint8_t*)0x4);
        EXPECT_TRUE(nested.m_generate_partial.expect_calls().with((uint8_t*)0x4));

        stack.seed(33);
        EXPECT_TRUE(nested.m_seed.expect_calls().with(33));

    }
}
