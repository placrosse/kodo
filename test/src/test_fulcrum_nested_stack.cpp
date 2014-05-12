// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>

#include <kodo/rlnc/fulcrum_nested_stack.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        class nested_dummy_stack
        {
        public:

            typedef boost::shared_ptr<nested_dummy_stack> pointer;

            class factory
            {
            public:

                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size)
                { }

                uint32_t max_symbols() const
                {
                    return m_max_symbols;
                }
                uint32_t max_symbol_size() const
                {
                    return m_max_symbol_size;
                }

                void set_symbols(uint32_t symbols)
                {
                    m_symbols = symbols;
                }
                void set_symbol_size(uint32_t symbol_size)
                {
                    m_symbol_size = symbol_size;
                }

                pointer build()
                {
                    return boost::make_shared<nested_dummy_stack>();
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

                uint32_t m_symbols;
                uint32_t m_symbol_size;

            };

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }

        };

        template<uint32_t MaxExpansion>
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

                uint32_t max_expansion() const
                {
                    return MaxExpansion;
                }

                uint32_t expansion() const
                {
                    return MaxExpansion;
                }

                uint32_t max_symbols() const
                {
                    return m_max_symbols;
                }
                uint32_t max_symbol_size() const
                {
                    return m_max_symbol_size;
                }

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

        template<uint32_t MaxExpansion>
        class dummy_stack
            : public fulcrum_nested_stack<
                nested_dummy_stack, dummy_layer<MaxExpansion> >
        { };

    }

}

TEST(TestFulcrumNestedStack, api)
{
    static const uint32_t max_expansion = 4;
    uint32_t max_symbols = 10;
    uint32_t max_symbol_size = 10;

    typedef kodo::dummy_stack<max_expansion> test_stack;

    test_stack::factory factory(max_symbols, max_symbol_size);

    EXPECT_EQ(factory.max_expansion(), max_expansion);
    EXPECT_EQ(factory.max_symbols(), max_symbols);
    EXPECT_EQ(factory.max_symbol_size(), max_symbol_size);

    auto& nested_factory = factory.nested();

    // Checking that the nested factory has max_symbols + the
    // expansion
    EXPECT_EQ(nested_factory.max_symbols(), max_symbols + max_expansion);
    EXPECT_EQ(nested_factory.max_symbol_size(), max_symbol_size);

    test_stack stack;
    stack.initialize(factory);

    auto& nested_stack = stack.nested();

    stack.initialize(factory);

    EXPECT_EQ(nested_factory.max_symbols(), max_symbols + max_expansion);
    EXPECT_EQ(nested_factory.max_symbol_size(), max_symbol_size);

}
