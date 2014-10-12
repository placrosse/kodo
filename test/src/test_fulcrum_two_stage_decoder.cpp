// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_fulcrum_two_stage_decoder.cpp Unit tests for the
/// fulcrum two stage combined decoder

#include <gtest/gtest.h>

#include <kodo/rlnc/fulcrum_two_stage_decoder.hpp>

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        class stage_decoder
        {
        public:

            class factory
            {
            public:

                typedef boost::shared_ptr<stage_decoder> pointer;

            public:

                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                pointer build()
                {
                    auto p = boost::make_shared<stage_decoder>();
                    p->m_symbol_size = m_symbol_size;
                    p->m_symbols = m_symbols;
                    return p;
                }

                void set_symbol_size(uint32_t symbol_size)
                {
                    m_symbol_size = symbol_size;
                }

                void set_symbols(uint32_t symbols)
                {
                    m_symbols = symbols;
                }

                void set_elimination_offset(uint32_t offset)
                {
                    (void) offset;
                }

                void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
                {
                    (void) symbol_data;
                    (void) coefficients;
                }

                uint32_t rank() const
                {
                    return 2U;
                }

                uint32_t symbols() const
                {
                    return m_symbols;
                }

                uint32_t m_symbol_size;
                uint32_t m_symbols;

            };

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size;
            }

            uint32_t coefficient_vector_size() const
            {
                return 2U;
            }

            uint32_t m_symbol_size;
            uint32_t m_symbols;

        };

        class dummy_layer
        {
        public:

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }


                uint32_t max_expansion() const
                {
                    return 2U;
                }

                uint32_t expansion() const
                {
                    return 2U;
                }

                uint32_t symbol_size() const
                {
                    return 2U;
                }

                uint32_t symbols() const
                {
                    return 2U;
                }

                uint32_t max_symbols() const
                {
                    return 2U;
                }

                uint32_t max_symbol_size() const
                {
                    return 2U;
                }

                uint32_t max_coefficient_vector_size() const
                {
                    return 2U;
                }

            };

        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void)the_factory;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void)the_factory;
            }

            uint32_t max_expansion() const
            {
                return 2U;
            }


            uint32_t symbol_size() const
            {
                return 2U;
            }

            uint32_t max_symbol_size() const
            {
                return 2U;
            }

            uint32_t max_coefficient_vector_size() const
            {
                return 2U;
            }


            uint32_t max_symbols() const
            {
                return 2U;
            }

            uint32_t symbols() const
            {
                return 2U;
            }

            uint32_t expansion() const
            {
                return 2U;
            }

            void decode_symbol(uint8_t* symbol_data, uint8_t* coefficients)
            {
                (void) symbol_data;
                (void) coefficients;
            }

            void map(const uint8_t* outer_coefficients,
                     uint8_t* inner_coefficients)
            {
                (void) outer_coefficients;
                (void) inner_coefficients;
            }



        };

        class dummy_stack :
            public fulcrum_two_stage_decoder<stage_decoder,
                                             stage_decoder,
                                             dummy_layer>
        { };

    }
}

/// Run the tests typical coefficients stack
TEST(TestFulcrumTwoStageDecoder, api)
{
    kodo::dummy_stack::factory_base factory(10,10);

    kodo::dummy_stack stack;
    stack.construct(factory);
    stack.initialize(factory);

    auto stage1 = factory.build_stage_one();
    auto stage2 = factory.build_stage_two();

    //@todo Add appropriate tests here..
}
