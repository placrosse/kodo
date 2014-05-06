// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <sak/storage.hpp>

namespace kodo
{

    /// @todo ingroup + test + review
    ///
    /// @brief This precoder uses a systematic structure i.e. all
    ///        original source symbols added to the outer code is
    ///        first added to the inner code. After this the outer
    ///        code is used to encode a number of symbols added as the
    ///        extension.
    template<class SuperCoder>
    class systematic_precoder : public SuperCoder
    {
    public:

        /// @copydoc layer::construct(Factory &)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_temp_coefficients.resize(
                the_factory.max_coefficient_vector_size());

            m_expansion_symbols.resize(the_factory.max_expansion());
            for(auto& v: m_expansion_symbols)
            {
                v.resize(the_factory.max_symbol_size());
            }
        }

        /// @copydoc precoder::precode()
        void precode()
        {
            // Fetch the inner encoder
            auto& nested = SuperCoder::nested();

            assert(nested);
            assert(nested->symbol_size() == SuperCoder::symbol_size());
            assert(!nested->is_symbols_initialized());
            assert(SuperCoder::is_symbols_initialized());
            assert(SuperCoder::inner_symbols() == nested->symbols());

            // First add all the original source symbols
            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                sak::const_storage outer_symbol = sak::storage(
                    SuperCoder::symbol(i), SuperCoder::symbol_size());

                nested->set_symbol(i, outer_symbol);
            }

            // The index of the expansion starts after the original symbols
            uint32_t i = SuperCoder::symbols();

            for(uint32_t j = 0; j < SuperCoder::expansion(); ++j)
            {
                SuperCoder::map_to_outer(i, &m_temp_coefficients[0]);

                auto& expansion_symbol = m_expansion_symbols[j];

                SuperCoder::encode_symbol(
                    &expansion_symbol[0], &m_temp_coefficients[0]);

                sak::const_storage outer_symbol = sak::storage(
                    &expansion_symbol[0], SuperCoder::symbol_size());

                nested->set_symbol(i, outer_symbol);

                ++i;
            }

            assert(nested->is_symbols_initialized());
        }

    protected:

        /// Temporary buffer for symbol headers of the outer code
        std::vector<uint8_t> m_temp_coefficients;

        // Storage for the outer code symbols
        std::vector<std::vector<uint8_t> > m_expansion_symbols;

    };

}
