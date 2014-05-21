// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <thread>
#include <sak/convert_endian.hpp>

#include "rlnc/full_rlnc_codes.hpp"

namespace kodo
{
    template<uint32_t Threads, class Stack, class SuperCoder>
    class thread_decoder_impl : public SuperCoder
    {
    public:

        /// The factory layer associated with this coder.
        /// In this case only needed to provide the max_payload_size()
        /// function.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size),
                  m_stack_factory(max_symbols, max_symbol_size / Threads)
            {
                assert(max_symbol_size > 0);
                assert((max_symbol_size % Threads) == 0);
            }

        protected:

            friend class thread_decoder_impl;

            typename Stack::factory& stack_factory()
            {
                return m_stack_factory;
            }

            typename Stack::factory m_stack_factory;
        };

    public:

        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_coefficients.resize(Threads);
            for(auto& c : m_coefficients)
            {
                c.resize(the_factory.max_coefficient_vector_size());
            }
        }

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_stacks.clear();

            auto& stack_factory = the_factory.stack_factory();
            stack_factory.set_symbols(the_factory.symbols());

            uint32_t symbol_size = the_factory.symbol_size();

            assert((symbol_size % Threads) == 0);

            uint32_t thread_symbol_size = symbol_size / Threads;
            assert(thread_symbol_size * Threads == symbol_size);

            stack_factory.set_symbol_size(thread_symbol_size);

            for(uint32_t i = 0; i < Threads; ++i)
            {
                m_stacks.push_back(stack_factory.build());
            }
        }

        void set_symbols(const sak::mutable_storage &symbol_storage)
        {
            SuperCoder::set_symbols(symbol_storage);

            uint32_t thread_symbol_size = SuperCoder::symbol_size() / Threads;
            assert(thread_symbol_size * Threads == SuperCoder::symbol_size());


            for(uint32_t i = 0; i < Threads; ++i)
            {

                assert(m_stacks[i]->symbols() == SuperCoder::symbols());

                for(uint32_t j = 0; j < SuperCoder::symbols(); ++j)
                {
                    uint8_t* symbol_j = SuperCoder::symbol(j);
                    auto s = sak::storage(symbol_j + (i * thread_symbol_size),
                                          thread_symbol_size);

                    m_stacks[i]->set_symbol(j, s);
                }
            }


        }

        void decode_symbol(uint8_t *symbol_data,
                           uint8_t *symbol_coefficients)
        {

            for(auto& c: m_coefficients)
            {
                std::copy_n(symbol_coefficients,
                          SuperCoder::coefficient_vector_size(),
                          c.data());
            }

            std::thread t[Threads];


            for(uint32_t i = 0; i < Threads; ++i)
            {
                uint8_t* thread_symbol =
                    symbol_data + (i*m_stacks[i]->symbol_size());

                uint8_t* thread_coefficients = m_coefficients[i].data();

                auto& d = m_stacks[i];

                t[i] = std::thread([d, thread_symbol, thread_coefficients](){
                        assert(d);
                        d->decode_symbol(thread_symbol, thread_coefficients);});
            }

            for(uint32_t i = 0; i < Threads; ++i)
            {
                t[i].join();
            }
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data,
                           uint32_t symbol_index)
        {
            std::thread t[Threads];

            for(uint32_t i = 0; i < Threads; ++i)
            {
                uint8_t* thread_symbol =
                    symbol_data + (i*m_stacks[i]->symbol_size());

                auto& d = m_stacks[i];

                t[i] = std::thread([d, thread_symbol, symbol_index](){
                        assert(d);
                        d->decode_symbol(thread_symbol, symbol_index);});
            }

            for(uint32_t i = 0; i < Threads; ++i)
            {
                t[i].join();
            }


        }


        uint32_t rank()
        {
            return m_stacks[0]->rank();
        }

        bool is_complete() const
        {
            return m_stacks[0]->is_complete();
        }

        std::vector<typename Stack::pointer> m_stacks;
        std::vector<std::vector<uint8_t> > m_coefficients;
    };


    template<class Field, class TraceTag = kodo::disable_trace>
    class thread_decoder : public
        // Payload API
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Decoder API
        thread_decoder_impl<4, shallow_full_rlnc_decoder<Field, TraceTag>,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        mutable_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        thread_decoder<Field, TraceTag>
        > > > > > > > > >
    { };



}
