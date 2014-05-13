// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_systematic_precoder.cpp Unit tests for the
///       systematic_precoder class

#include <cstdint>
#include <boost/make_shared.hpp>
#include <gtest/gtest.h>
#include <kodo/systematic_precoder.hpp>
#include <sak/storage.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        class nested_stack
        {
        public:
            uint32_t symbol_size()
            {
                return 10;
            }

            bool is_symbols_initialized()
            {
                return m_initialized;
            }

            uint32_t symbols()
            {
                return 10;
            }

            void set_symbol(uint32_t index, sak::const_storage symbol_data)
            {
                (void)index;
                (void)symbol_data;
                m_initialized = true;
            }

            bool m_initialized = false;
        };

        class dummy_layer
        {
        public:

            dummy_layer()
            : m_nested_stack(boost::make_shared<nested_stack>())
            {
                m_data.resize(10);
            }

            typedef boost::shared_ptr<nested_stack> pointer;

            class factory
            {
            public:

                uint32_t max_coefficient_vector_size() const
                {
                    return 10;
                }

                uint32_t max_expansion()
                {
                    return 10;
                }

                uint32_t max_symbol_size()
                {
                    return 10;
                }

            };

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void)the_factory;
            }

            const pointer& nested() const
            {
                return m_nested_stack;
            }

            pointer& nested()
            {
                return m_nested_stack;
            }

            uint32_t symbol_size()
            {
                return 10;
            }

            bool is_symbols_initialized()
            {
                return true;
            }

            uint32_t inner_symbols()
            {
                return 10;
            }

            uint32_t symbols()
            {
                return 10;
            }

            uint8_t* symbol(uint32_t index)
            {
                (void)index;
                return m_data.data();
            }

            uint32_t expansion()
            {
                return 10;
            }

            void map_to_outer(uint32_t index, uint8_t* coefficients)
            {
                (void)index;
                (void)coefficients;
            }

            void encode_symbol(uint8_t* expansion_symbol, uint8_t* coefficients)
            {
                (void)expansion_symbol;
                (void)coefficients;
            }

            std::vector<uint8_t> m_data;
            pointer m_nested_stack;
        };

        class dummy_stack :
            public systematic_precoder<dummy_layer>
        { };
    }

}

TEST(TestSystematicPrecoder, api)
{
    typedef kodo::dummy_stack test_stack;
    test_stack stack;
    test_stack::factory factory;

    stack.construct(factory);
    stack.precode();
}
