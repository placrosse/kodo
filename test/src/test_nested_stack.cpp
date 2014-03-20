// Copyright Steinwurf ApS 2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <memory>

#include <gtest/gtest.h>

#include <kodo/nested_stack.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        class dummy_nested
        {
        public:

            typedef std::shared_ptr<dummy_nested> pointer;

            class factory
            {
            public:

                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size),
                      m_symbols(0),
                      m_symbol_size(0)
                { }

                void set_symbols(uint32_t symbols)
                {
                    m_symbols = symbols;
                }

                void set_symbol_size(uint32_t symbol_size)
                {
                    m_symbol_size = symbol_size;
                }

                std::shared_ptr<dummy_nested> build()
                {
                    return std::make_shared<dummy_nested>();
                }

            public:

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;
                uint32_t m_symbols;
                uint32_t m_symbol_size;

            };

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_symbols = the_factory.m_symbols;
                m_symbol_size = the_factory.m_symbol_size;
            }

            uint32_t m_symbols;
            uint32_t m_symbol_size;

        };

        class dummy_layer
        {
        public:

            class factory
            {
            public:

                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t symbols() const
                {
                    return m_max_symbols;
                }

                uint32_t symbol_size() const
                {
                    return m_max_symbol_size;
                }


                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

            };

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }
        };

        class dummy_stack : public nested_stack<dummy_nested, dummy_layer>
        { };

    }

}


TEST(TestNestedStack, api)
{
    uint32_t symbols = 16;
    uint32_t symbol_size = 1400;

    kodo::dummy_stack::factory factory(symbols, symbol_size);
    EXPECT_EQ(factory.m_max_symbols, symbols);
    EXPECT_EQ(factory.m_max_symbol_size, symbol_size);

    auto& nested_factory = factory.nested();
    EXPECT_EQ(nested_factory.m_max_symbols, symbols);
    EXPECT_EQ(nested_factory.m_max_symbol_size, symbol_size);

    kodo::dummy_stack stack;

    stack.initialize(factory);

    auto nested = stack.nested();
    assert(nested);

    EXPECT_EQ(nested->m_symbols, symbols);
    EXPECT_EQ(nested->m_symbol_size, symbol_size);

}

