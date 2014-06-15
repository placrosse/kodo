// Copyright Steinwurf ApS 2014
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing
#pragma once

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

        /// Stack used as the "nested" stack
        class helper_nested_stack
        {
        public:

            typedef std::shared_ptr<helper_nested_stack> pointer;

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
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

                std::shared_ptr<helper_nested_stack> build()
                {
                    auto ptr = std::make_shared<helper_nested_stack>();
                    ptr->initialize(*this);
                    return ptr;
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

        /// Dummy layer used as SuperCoder for the nested_stack
        class helper_nested_layer
        {
        public:

            class factory_base
            {
            public:

                factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_symbols(max_symbols),
                      m_max_symbol_size(max_symbol_size),
                      m_symbols(max_symbols),
                      m_symbol_size(max_symbol_size)
                { }

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
                    return m_symbols;
                }

                uint32_t symbol_size() const
                {
                    return m_symbol_size;
                }

                uint32_t m_max_symbols;
                uint32_t m_max_symbol_size;

                uint32_t m_symbols;
                uint32_t m_symbol_size;

            };

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
            }


            template<class Factory>
            void initialize(Factory& the_factory)
            {
                m_symbols = the_factory.symbols();
                m_symbol_size = the_factory.symbol_size();
            }

            uint32_t symbols() const
            {
                return m_symbols;
            }

            uint32_t symbol_size() const
            {
                return m_symbol_size;
            }

            uint32_t m_symbols;
            uint32_t m_symbol_size;
        };

        /// The stack used in unit test
        class helper_test_nested_stack
            : public nested_stack<helper_nested_stack, helper_nested_layer>
        { };

    }

}
