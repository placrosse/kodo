// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_default_on_systematic_encoder.cpp Unit tests for the
///       kodo::default_on_systematic_encoder layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/default_on_systematic_encoder.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // default_on_systematic_encoder layer.
        struct dummy_layer
        {
        public:

            struct factory
            {
                /// @copydoc layer::factory::factory(uint32_t,uint32_t)
                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t max_symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t max_symbol_size() const
                {
                    return m_max_symbol_size;
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

            };

        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_symbols = the_factory.symbols();
                m_is_symbol_pivot.resize(m_symbols, false);
            }

            uint32_t encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
            {
                (void) symbol_data;
                (void) symbol_index;
                return 0;
            }

            uint32_t encode(uint8_t *symbol_data, uint8_t *symbol_header)
            {
                (void) symbol_data;
                (void) symbol_header;
                return 0;
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            bool is_symbol_pivot(uint32_t index) const
            {
                (void) index;
                return m_is_symbol_pivot[index];
            }

            uint32_t rank() const
            {
                uint32_t rank = 0;
                for(uint32_t i = 0; i < m_symbols; ++i)
                {
                    rank += m_is_symbol_pivot[i];
                }
                return rank;
            }

            uint32_t header_size() const
            {
                return 0;
            }

            uint32_t m_symbols;
            std::vector<bool> m_is_symbol_pivot;


            // bool m_in_systematic_phase;
        };

        // Instantiate a stack containing the default_on_systematic_encoder
        class dummy_stack
            : public default_on_systematic_encoder<dummy_layer>
          { };
    }
}

template<bool SystematicOn, class Stack>
inline void test()
{
    typedef Stack stack_type;
    typedef typename Stack::factory factory_type;

    stack_type stack;
    factory_type factory(10, 10);

    stack.construct(factory);
    stack.initialize(factory);

    std::vector<uint8_t> header(stack.header_size());
    std::vector<uint8_t> symbol(0);//stack.symbol_size());

    stack.encode(&symbol[0], &header[0]);

}

/// Run the tests typical coefficients stack
TEST(TestDefaultOnSystematicEncoder, api)
{
    test<true, kodo::dummy_stack>();
}
