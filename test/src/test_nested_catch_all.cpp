// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_nested_catch_all.cpp Unit tests for the nested catch all

#include <gtest/gtest.h>

#include <kodo/nested_catch_all.hpp>

namespace sak
{
    bool operator==(const sak::const_storage& a, const sak::const_storage& b)
    {
        return equal(a,b);
    }
}


// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{
    template<class R>
    class return_handler
    {
    public:

        return_handler()
            : m_position(0)
        {}

        void set_return(const R& value)
        {
            m_returns.push_back(value);
        }

        void set_returns(const std::initializer_list<R>& values)
        {
            m_returns.insert(m_returns.end(),
                             values.begin(), values.end());
        }

        R operator()() const
        {
            assert(m_position < m_returns.size());

            R value = m_returns[m_position];
            ++m_position;

            return value;
        }

        mutable uint32_t m_position;
        std::vector<R> m_returns;
    };

    template<>
    class return_handler<void>
    {
    public:

        void operator()() const
        {
        }
    };

    template<class... Args>
    class call_handler
    {
    public:

        using arguments = std::tuple<Args...>;

        void add_call(Args... args) const
        {
            m_calls.emplace_back(args...);
        }

        bool called_with(Args... args) const
        {
            if (m_calls.size() != 1U)
            {
                return false;
            }

            return called_with(args...);
        }

        bool called_once_with(Args... args) const
        {
            assert(m_calls.size() > 0);

            // We know there is one element in the list
            const auto& stored_args = m_calls.back();
            const auto& reference_args = std::make_tuple(args...);

            return stored_args == reference_args;
        }

        mutable std::vector<arguments> m_calls;
    };

    template<typename T> class call;

    template<typename R, typename... Args> class call<R (Args...)>
    {
    public:

        R operator()(Args... args) const
        {
            m_call_handler.add_call(args...);
            return m_return_handler();
        }

        template<class T>
        void set_return(const T& return_value)
        {
            m_return_handler.set_return(return_value);
        }

        template<class T>
        void set_returns(const std::initializer_list<T> &returns)
        {
            m_return_handler.set_returns(returns);
        }

        bool called_once_with(Args... args) const
        {
            return m_call_handler.called_once_with(args...);
        }

        bool called_with(Args... args) const
        {
            return m_call_handler.called_with(args...);
        }


        return_handler<R> m_return_handler;
        call_handler<Args...> m_call_handler;
    };


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
                return 2U;
            }

            uint32_t max_block_size() const
            {
                return 3U;
            }

            uint32_t max_header_size() const
            {
                return 4U;
            }

            uint32_t max_id_size() const
            {
                return 5U;
            }

            uint32_t max_payload_size() const
            {
                return 6U;
            }

            uint32_t max_coefficient_vector_size() const
            {
                return 7U;
            }

            uint32_t symbols() const
            {
                return 8U;
            }

            uint32_t symbol_size() const
            {
                return 9U;
            }

            call<uint32_t ()> m_max_symbols;

        };

    public:


        void copy_symbols(const sak::mutable_storage& dest) const
        {
            m_copy_symbols(dest);
        }

        call<void (const sak::mutable_storage& dest)> m_copy_symbols;



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

    public: // The nested stack accessors

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

        EXPECT_EQ(factory.max_symbol_size(), 2U);
        EXPECT_EQ(factory.max_block_size(), 3U);
        EXPECT_EQ(factory.max_header_size(), 4U);
        EXPECT_EQ(factory.max_id_size(), 5U);
        EXPECT_EQ(factory.max_payload_size(), 6U);
        EXPECT_EQ(factory.max_coefficient_vector_size(), 7U);
        EXPECT_EQ(factory.symbols(), 8U);
        EXPECT_EQ(factory.symbol_size(), 9U);

    }

    // Test the stack
    {
        dummy_stack stack;

        auto& nested = stack.m_nested;

        std::vector<uint8_t> data(200, 'z');

        stack.copy_symbols(sak::storage(data));
        EXPECT_TRUE(nested.m_copy_symbols.called_once_with(
                        sak::storage(data)));
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
