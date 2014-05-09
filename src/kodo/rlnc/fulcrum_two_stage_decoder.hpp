// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace kodo
{
    /// @todo tests + docs
    template<class StageOneDecoder, class StageTwoDecoder, class SuperCoder>
    class fulcrum_two_stage_decoder : public SuperCoder
    {
    public:

        /// The stage one decoder
        typedef StageOneDecoder stage_one_decoder;

        /// The stage one decoder factory
        typedef typename stage_one_decoder::factory stage_one_factory;

        /// The stage two decoder
        typedef StageTwoDecoder stage_two_decoder;

        /// The stage two decoder factory
        typedef typename stage_two_decoder::factory stage_two_factory;

    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory(max_symbols, max_symbol_size),
                m_stage_one_factory(SuperCoder::factory::max_expansion() +
                                    max_symbols,
                                    max_symbol_size),
                m_stage_two_factory(SuperCoder::factory::max_expansion() +
                                    max_symbols,
                                    max_symbol_size)
            { }

            typename stage_one_decoder::pointer build_stage_one()
            {
                uint32_t symbol_size = SuperCoder::factory::symbol_size();
                uint32_t symbols = SuperCoder::factory::expansion();
                uint32_t offset = SuperCoder::factory::symbols();

                m_stage_one_factory.set_symbol_size(symbol_size);
                m_stage_one_factory.set_symbols(symbols);
                m_stage_one_factory.set_elimination_offset(offset);

                return m_stage_one_factory.build();
            }

            typename stage_two_decoder::pointer build_stage_two()
            {
                uint32_t symbol_size = SuperCoder::factory::symbol_size();
                uint32_t symbols = SuperCoder::factory::symbols() +
                    SuperCoder::factory::expansion();

                m_stage_two_factory.set_symbol_size(symbol_size);
                m_stage_two_factory.set_symbols(symbols);

                return m_stage_two_factory.build();
            }

        private:

            /// The stage one factory
            stage_one_factory m_stage_one_factory;

            /// The stage two factory
            stage_two_factory m_stage_two_factory;

        };

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_stage_one_decoder_copied.resize(
                the_factory.max_expansion() );

            m_stage_two_decoder_copied.resize(
                the_factory.max_expansion() + the_factory.max_symbols() );

            m_outer_coefficients.resize(
                the_factory.max_coefficient_vector_size());

            m_outer_symbol.resize(
                the_factory.max_symbol_size());
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_stage_one_decoder = the_factory.build_stage_one();
            m_stage_two_decoder = the_factory.build_stage_two();

            assert(m_stage_one_decoder->symbols() ==
                   SuperCoder::expansion());

            assert(m_stage_two_decoder->symbols() ==
                   SuperCoder::expansion() + SuperCoder::symbols());

            assert(m_stage_one_decoder->symbol_size() ==
                   m_stage_two_decoder->symbol_size());

            assert(m_stage_one_decoder->symbol_size() ==
                   SuperCoder::symbol_size());

            assert(m_stage_one_decoder->coefficient_vector_size() ==
                   m_stage_two_decoder->coefficient_vector_size());

            std::fill_n(m_stage_two_decoder_copied.begin(),
                        m_stage_two_decoder->symbols(), false);

            std::fill_n(m_stage_one_decoder_copied.begin(),
                        m_stage_one_decoder->symbols(), false);
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            if(SuperCoder::is_outer_systematic())
            {

                if(symbol_index < SuperCoder::symbols())
                {
                    m_stage_one_decoder_copied[symbol_index] = true;
                    SuperCoder::decode_symbol(symbol_data, symbol_index);
                }

                m_stage_two_decoder->decode_symbol(
                    symbol_data, symbol_index);
            }
            else
            {
                assert(0);
            }
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            uint32_t rank_stage_one = m_stage_one_decoder->rank();

            // We always pass the packet to the stage one decoder it
            // is responsible for eliminating the extension.
            m_stage_one_decoder->decode_symbol(symbol_data, coefficients);

            if(m_stage_one_decoder->rank() > rank_stage_one)
            {
                // If the rank increased in the stage one coder we
                // check whether we have achieved full rank. We do not
                // need to go to the stage two decoder since we have
                // accumulated the additional degree of freedom
                // contained in the encoded symbol.
                check_combined_rank();
                return;
            }

            // Getting here means that the stage one decoder did not
            // increase its rank, next step is to pass the reduced
            // symbol to the stage two decoder
            uint32_t rank_stage_two = m_stage_two_decoder->rank();

            m_stage_two_decoder->decode_symbol(symbol_data, coefficients);

            if(m_stage_two_decoder->rank() > rank_stage_two)
            {
                // If the stage two decoder increased the rank we
                // check whether we can map anything to the outer
                // decoder
                check_combined_rank();
            }

        }

        /// This function is checking the combined rank of the stage
        /// one and stage two decoders. If the rank is sufficient we
        /// may be able to decode the outer code we map the symbols,
        /// that have not previously, been mapped to the outer code.
        void check_combined_rank()
        {
            uint32_t combined_rank = m_stage_one_decoder->rank() +
                m_stage_two_decoder->rank();

            if(combined_rank < SuperCoder::symbols())
                return;

            // We map from stage two first. The is advantageous if the
            // outer encoder has produced systematic symbols. Since in
            // that case we are able to map symbols directly between the
            // outer and inner decoders.
            for(uint32_t i = 0; i < m_stage_two_decoder->symbols(); ++i)
            {
                bool is_pivot = m_stage_two_decoder->is_symbol_pivot(i);
                bool is_copied = m_stage_two_decoder_copied[i];

                if(is_pivot && !is_copied)
                {
                    m_stage_two_decoder_copied[i] = true;
                    map_stage_two(i);
                }
            }


            for(uint32_t i = 0; i < m_stage_one_decoder->symbols(); ++i)
            {
                bool is_pivot = m_stage_one_decoder->is_symbol_pivot(i);
                bool is_copied = m_stage_one_decoder_copied[i];

                if(is_pivot && !is_copied)
                {
                    m_stage_one_decoder_copied[i] = true;
                    map_stage_one(i);
                }
            }

        }

        void map_stage_one(uint32_t symbol_index)
        {
            const uint8_t* symbol_data =
                m_stage_one_decoder->symbol(symbol_index);

            const uint8_t* coefficients =
                m_stage_one_decoder->coefficient_vector_data(symbol_index);

            map_symbol(symbol_data, coefficients);
        }

        void map_stage_two(uint32_t symbol_index)
        {
            const uint8_t* symbol_data =
                m_stage_two_decoder->symbol(symbol_index);

            const uint8_t* coefficients =
                m_stage_two_decoder->coefficient_vector_data(symbol_index);

            map_symbol(symbol_data, coefficients);
        }

        void map_symbol(const uint8_t* symbol_data,
                        const uint8_t* coefficients)
        {
            // Copy the pivot symbol to a modifiable buffer
            std::copy_n(symbol_data, SuperCoder::symbol_size(),
                        &m_outer_symbol[0]);

            SuperCoder::map_to_outer(
                coefficients, &m_outer_coefficients[0]);

            SuperCoder::decode_symbol(
                &m_outer_symbol[0], &m_outer_coefficients[0]);
        }

    private:

        /// The stage one decoder
        typename stage_one_decoder::pointer m_stage_one_decoder;

        /// The stage two decoder
        typename stage_two_decoder::pointer m_stage_two_decoder;

        /// Keeps track of which symbols have been copied from the
        /// stage one decoder to the inner decoder
        std::vector<bool> m_stage_one_decoder_copied;

        /// Keeps track of which symbols have been copied from the
        /// stage two decoder to the inner decoder
        std::vector<bool> m_stage_two_decoder_copied;

        /// Buffer for the outer coding coefficients
        std::vector<uint8_t> m_outer_coefficients;

        /// Buffer for the outer coding symbol data
        std::vector<uint8_t> m_outer_symbol;

    };

}
