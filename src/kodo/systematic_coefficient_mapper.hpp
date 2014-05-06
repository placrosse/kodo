// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <algorithm>

#include <fifi/fifi_utils.hpp>
#include <sak/aligned_allocator.hpp>

namespace kodo
{

    /// @todo ingroup + test + review
    ///
    /// @brief The role of the coefficient mapper is to produce the
    /// mapping between a symbol in the inner code and a symbol in the
    /// outer code.  The coefficient mapper controls which symbols are
    /// coded in the outer code and
    template<class SuperCoder>
    class systematic_coefficient_mapper : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

    public:

        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            // Prepare buffer for the coefficients of the outer code
            m_coefficients_lookup.resize(the_factory.max_expansion());

            for(uint32_t i = 0; i < m_coefficients_lookup.size(); ++i)
            {
                auto& v = m_coefficients_lookup[i];
                v.resize(the_factory.max_coefficient_vector_size());

            }
        }

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            for(uint32_t i = 0; i < m_coefficients_lookup.size(); ++i)
            {
                auto& v = m_coefficients_lookup[i];
                SuperCoder::seed(i);
                SuperCoder::generate(&v[0]);
            }
        }

        void map_to_outer(const uint8_t* inner_coefficients,
                          uint8_t* outer_coefficients)
        {
            assert(inner_coefficients);
            assert(outer_coefficients);

            auto* dest = reinterpret_cast<value_type*>(outer_coefficients);

            assert(dest != 0);

            // Zero the destination coefficients buffer
            std::fill_n(dest, SuperCoder::coefficient_vector_length(), 0U);

            for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
            {
                auto coefficient = fifi::get_value<fifi::binary>(
                    inner_coefficients, i);

                if(!coefficient)
                    continue;

                fifi::set_value<field_type>(dest, i, 1U);
            }

            // Loop over the inner coding coefficients and create the outer
            // coding coefficients
            for(uint32_t i = 0; i < SuperCoder::expansion(); ++i)
            {
                auto coefficient_index = i + SuperCoder::symbols();
                assert(i < SuperCoder::inner_symbols());

                auto coefficient = fifi::get_value<fifi::binary>(
                    inner_coefficients, coefficient_index);

                if(!coefficient)
                    continue;

                assert(i < m_coefficients_lookup.size());

                const auto& v = m_coefficients_lookup[i];
                const auto* src = reinterpret_cast<const value_type*>(&v[0]);

                assert(src != 0);

                SuperCoder::add(
                    dest, src, SuperCoder::coefficient_vector_length());
            }

        }

        /// Given a symbol from the inner code what will be the
        /// coefficients from the outer code
        void map_to_outer(uint32_t inner_symbol, uint8_t* outer_coefficients)
        {
            assert(inner_symbol < SuperCoder::inner_symbols());
            assert(outer_coefficients);

            auto* dest = reinterpret_cast<value_type*>(outer_coefficients);

            if(inner_symbol < SuperCoder::symbols())
            {
                // Zero the destination coefficients buffer
                std::fill_n(dest, SuperCoder::coefficient_vector_length(), 0U);
                fifi::set_value<field_type>(dest, inner_symbol, 1U);
            }
            else
            {
                // Get the offset into the extension
                uint32_t offset = inner_symbol - SuperCoder::symbols();

                assert(offset < SuperCoder::expansion());
                assert(offset < m_coefficients_lookup.size());

                const auto& v = m_coefficients_lookup[offset];
                const auto* src = reinterpret_cast<const value_type*>(&v[0]);

                std::copy_n(src, SuperCoder::coefficient_vector_length(), dest);
            }
        }

        /// Given a specific symbol from the inner code is the
        /// corresponding symbol in the outer code also systematic
        /// @param inner_symbol The index of the symbol in the inner code
        /// @return True if the corresponding outer symbol is systematic
        bool is_outer_systematic() const
        {
            return true;
        }

    private:

        /// The buffer type
        typedef std::vector<uint8_t, sak::aligned_allocator<uint8_t> >
            aligned_vector;

        /// Coefficients use for the inner code
        std::vector<aligned_vector> m_coefficients_lookup;

    };
}
