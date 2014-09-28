// Copyright Steinwurf ApS 2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <algorithm>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/optional.hpp>

#include <fifi/fifi_utils.hpp>

namespace kodo
{
    /// @ingroup coefficient_generator_layers
    /// @brief Generate perpetual coefficients with a specific width
    template<class SuperCoder>
    class perpetual_generator : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

        /// The random generator used
        typedef boost::random::mt19937 generator_type;

        /// @copydoc layer::seed_type
        typedef generator_type::result_type seed_type;

    public:

        /// Constructor
        perpetual_generator() :
            m_symbol_distribution(field_type::min_value, field_type::max_value),
            m_pivot_distribution(),
            m_pre_charging(false),
            m_pseudo_systematic(false),
            m_width_ratio(0.1)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            SuperCoder::initialize(the_factory);

            assert(SuperCoder::symbols() > 0);

            m_pivot_distribution =
                boost::random::uniform_int_distribution<uint32_t>(
                    0, SuperCoder::symbols() - 1);

            m_width =
                std::max(1U, (uint32_t)(SuperCoder::symbols() * m_width_ratio));
            m_generated = 0;

            assert(m_width <= SuperCoder::symbols());
        }

        /// @copydoc layer::generate(uint8_t*)
        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            // Ensure all coefficients are initially zero
            std::fill_n(coefficients, SuperCoder::coefficient_vector_size(), 0);

            uint32_t pivot;
            if (m_pre_charging && m_generated < m_width + 1)
            {
                pivot = 0;
            }
            else if (m_pseudo_systematic &&
                     m_generated < SuperCoder::symbols() + m_width)
            {
                pivot = m_generated - m_width;
            }
            else
            {
                // Random pivot selection is the default policy
                pivot = m_pivot_distribution(m_random_generator);
            }

            value_type* c = reinterpret_cast<value_type*>(coefficients);
            fifi::set_value<field_type>(c, pivot, 1);

            uint32_t symbols = SuperCoder::symbols();
            for (uint32_t i = 1; i <= m_width; ++i)
            {
                uint32_t index = (pivot + i) % symbols;
                value_type coefficient =
                    m_symbol_distribution(m_random_generator);
                fifi::set_value<field_type>(c, index, coefficient);
            }

            m_generated++;
        }

        /// @copydoc layer::generate_partial(uint8_t*)
        void generate_partial(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            // Ensure all coefficients are initially zero
            std::fill_n(coefficients, SuperCoder::coefficient_vector_size(), 0);

            // No data to recode return the valid zero symbol and vector.
            if (SuperCoder::rank() == 0)
                return;

            uint32_t symbols = SuperCoder::symbols();

            //draw a valid pivot
            uint32_t pivot = m_pivot_distribution(m_random_generator);
            while (!SuperCoder::is_symbol_pivot(pivot))
                pivot = m_pivot_distribution(m_random_generator);

            value_type* c = reinterpret_cast<value_type*>(coefficients);
            fifi::set_value<field_type>(c, pivot, 1);

            for (uint32_t i = 1; i <= m_width; ++i)
            {
                uint32_t index = (pivot + i) % symbols;
                if (!SuperCoder::is_symbol_pivot(index))
                {
                    continue;
                }

                value_type coefficient =
                    m_symbol_distribution(m_random_generator);

                fifi::set_value<field_type>(c, index, coefficient);
            }
        }

        /// Generate a local recoding vector that ensures that the resulting
        /// coding vector after recoding does not have a width higher than what
        /// was defined for this code
        /// @param coefficients the buffer for the recoding vector
        void generate_recode(uint8_t * coefficients)
        {
            assert(coefficients != 0);

            // Ensure all coefficients are initially zero
            std::fill_n(coefficients, SuperCoder::coefficient_vector_size(), 0);

            // No data to recode return the valid zero symbol and vector.
            if (SuperCoder::rank() == 0)
                return;

            uint32_t symbols = SuperCoder::symbols();

            // Draw a valid pivot
            uint32_t pivot = m_pivot_distribution(m_random_generator);
            while (!SuperCoder::is_symbol_pivot(pivot))
                pivot = m_pivot_distribution(m_random_generator);

            value_type *coefficients_index =
                SuperCoder::coefficient_vector_values(pivot);
            boost::optional<uint32_t> back =
                find_back(coefficients_index, (pivot + m_width) % symbols);

            // The search must yield a valid back index
            assert(back != boost::none);

            uint32_t max_back = back.get();

            value_type* c = reinterpret_cast<value_type*>(coefficients);
            fifi::set_value<field_type>(c, pivot, 1);

            // Check which of the w rows after the pivot can be used
            for (uint32_t i = 1 ; i <= m_width; ++i)
            {
                uint32_t index = (pivot + i) % symbols;

                if (!SuperCoder::is_symbol_pivot(index))
                    continue;

                coefficients_index =
                    SuperCoder::coefficient_vector_values(index);

                boost::optional<uint32_t> back =
                    find_back(coefficients_index, (index + m_width) % symbols);

                if (back == boost::none)
                    continue;

                // if the last non-zero element in the coding vector under
                // consideration protrudes beyond the maximum width defined it
                // is not used
                if (calc_width(pivot, back.get()) > m_width)
                    continue;

                if (calc_width(pivot, back.get()) > calc_width(pivot, max_back))
                    max_back = back.get();

                value_type coefficient =
                    m_symbol_distribution(m_random_generator);

                fifi::set_value<field_type>(c, index, coefficient);
            }

            // Check which of the (up to) w rows before the pivot can be used
            for (uint32_t i = 1 ; i <= m_width; ++i)
            {
                uint32_t index = (pivot + symbols - i) % symbols;

                if (!SuperCoder::is_symbol_pivot(index))
                    continue;

                coefficients_index =
                    SuperCoder::coefficient_vector_values(index);

                boost::optional<uint32_t> back =
                    find_back(coefficients_index, (index + m_width) % symbols);

                if (back == boost::none)
                    continue;

                // None of the indices before will fit in the width
                if (calc_width(index, max_back) > m_width)
                    break;

                pivot = index;

                if (calc_width(pivot, back.get()) > calc_width(pivot, max_back))
                    max_back = back.get();

                value_type coefficient =
                    m_symbol_distribution(m_random_generator);

                fifi::set_value<field_type>(c, index, coefficient);
            }
        }

        /// @copydoc layer::seed(seed_type)
        void seed(seed_type seed_value)
        {
            m_random_generator.seed(seed_value);
        }

        /// Set the ratio that is used to calculate the number of
        /// non-zero coefficients after the pivot (i.e. the width)
        /// @param ratio the width ratio
        void set_width_ratio(double ratio)
        {
            assert(ratio > 0);
            assert(ratio < 1);

            // Save the ratio and recalculate the width
            m_width_ratio = ratio;
            m_width =
                std::max(1U, (uint32_t)(SuperCoder::symbols() * m_width_ratio));
        }

        /// Get the ratio that is used to calculate the width
        /// @return the width ratio of the generator
        double width_ratio() const
        {
            return m_width_ratio;
        }

        /// Set the number of non-zero coefficients after the pivot.
        /// Width ratio is recalculated from this value
        /// @param width the width
        void set_width(uint32_t width)
        {
            assert(width < SuperCoder::symbols());
            assert(width > 0);

            // Save the width and recalculate the width ratio
            m_width = width;
            m_width_ratio = (double)m_width / SuperCoder::symbols();
        }

        /// Get the width
        /// @return the width used by the generator
        uint32_t width() const
        {
            return m_width;
        }

        /// Set the pre-charging property of the generator
        /// @param pre_charging the new setting for pre-charging
        void set_pre_charging(bool pre_charging)
        {
            m_pre_charging = pre_charging;
        }

        /// Get the pre-charging property of the generator
        /// @return the current setting for pre-charging
        bool pre_charging() const
        {
            return m_pre_charging;
        }

        /// Set the pseudo-systematic property of the generator
        /// @param pseudo_systematic the new setting for pseudo-systematic
        void set_pseudo_systematic(bool pseudo_systematic)
        {
            m_pseudo_systematic = pseudo_systematic;
        }

        /// Get the pseudo-systematic property of the generator
        /// @return the current setting for pseudo-systematic
        bool pseudo_systematic() const
        {
            return m_pseudo_systematic;
        }

    protected:

        /// Calculate the width from the pivot to the last element, taking into
        /// consideration that the non-zero values may wrap around from the end
        /// of the vector to the start
        /// @param pivot the first non-zero element
        /// @param back the index of the last non-zero element
        uint32_t calc_width(uint32_t pivot, uint32_t back)
        {
            if (back >= pivot)
            {
                return back - pivot;
            }
            else
            {
                return back + SuperCoder::symbols() - pivot;
            }
        }

        /// find the last non-zero element after the pivot
        /// @param coefficients
        /// @param back_hint
        /// @return the index of the last non-zero element or boost::none if no
        /// meaningful index was identified
        boost::optional<uint32_t> find_back(
            value_type* coefficients,
            boost::optional<uint32_t> back_hint=boost::none)
        {
            uint32_t symbols = SuperCoder::symbols();

            // if no hint is provided we search from the last element
            uint32_t back;
            if (back_hint == boost::none)
            {
                back = symbols - 1;
            }
            else
            {
                back = back_hint.get();
            }

            assert(back < symbols);

            for (uint32_t i = 0; i < symbols; ++i)
            {
                uint32_t index = (back + symbols - i) % symbols;

                value_type coefficient =
                    SuperCoder::coefficient_value(coefficients, index);

                if (!coefficient)
                    continue;

                return boost::optional<uint32_t>(index);
            }

            // There is no meaningful last element for the zero vector
            return boost::none;
    }

    protected:

        /// The distribution wrapping the random generator
        boost::random::uniform_int_distribution<value_type>
            m_symbol_distribution;

        /// Generates uniform indices from the generation
        boost::random::uniform_int_distribution<uint32_t>
            m_pivot_distribution;

        /// The random generator
        boost::random::mt19937 m_random_generator;

        /// Indicates if pre-charging for pivot selection in the initial phase
        bool m_pre_charging;

        /// Indicates if pivot indices are selected in a pseudo-systematic
        /// fashion in the second phase
        bool m_pseudo_systematic;

        /// The ratio that is used to calculate the width
        double m_width_ratio;

        /// The number of non-zero values following the pivot
        uint32_t m_width;

        /// Counter for number of generated coefficient vectors
        uint32_t m_generated;
    };
}
