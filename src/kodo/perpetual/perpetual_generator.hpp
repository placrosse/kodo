// Copyright Steinwurf ApS 2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

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
        perpetual_generator()
            : m_symbol_distribution(field_type::min_value, field_type::max_value),
              m_pivot_distribution(0, SuperCoder::symbols()-1),
              m_width(SuperCoder::symbols()/2),
              m_generated(0)
            {}

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_pivot_distribution = boost::random::uniform_int_distribution
                <uint32_t>(0, SuperCoder::symbols()-1);
            m_width = SuperCoder::symbols()/2;
            m_generated = 0;

            assert(m_width < SuperCoder::symbols());
        }

        /// @copydoc layer::generate(uint8_t*)
        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            // Ensure all coefficients are initially zero
            std::fill_n( coefficients, SuperCoder::coefficient_vector_size(), 0);

            uint32_t pivot;
            if(m_generated < SuperCoder::symbols())
                pivot = m_generated;
            else
                pivot = m_pivot_distribution(m_random_generator);

            value_type* c = reinterpret_cast<value_type*>(coefficients);
            fifi::set_value<field_type>(c, pivot, 1);

            uint32_t symbols = SuperCoder::symbols();
            for(uint32_t i = 1; i <= m_width; ++i)
            {
                uint32_t index = (pivot + i) % symbols;
                value_type coefficient = m_symbol_distribution(m_random_generator);
                fifi::set_value<field_type>(c, index, coefficient);
            }

            m_generated++;
        }

        /// @copydoc layer::generate_partial(uint8_t*)
        void generate_partial(uint8_t *coefficients)
        {
            assert(coefficients != 0);
            m_pivot_distribution = boost::random::uniform_int_distribution<uint32_t>(0, SuperCoder::symbols()-1);

            // Ensure all coefficients are initially zero
            std::fill_n( coefficients, SuperCoder::coefficient_vector_size(), 0);

            uint32_t symbols = SuperCoder::symbols();

            //attempt to draw a valid pivot, we try at most symbols time
            uint32_t pivot = m_pivot_distribution(m_random_generator);
            uint32_t trials = 1;
            while((SuperCoder::is_symbol_pivot(pivot)) and (trials <= symbols))
            {
                pivot = m_pivot_distribution(m_random_generator);
                trials++;
            }

            value_type* c = reinterpret_cast<value_type*>(coefficients);
            if(SuperCoder::is_symbol_pivot(pivot))
            {
                fifi::set_value<field_type>(c, pivot, 1);
            }
            else
            {
                return;
            }

            for(uint32_t i = 1; i <= m_width; ++i)
            {
                uint32_t index = (pivot + i) % symbols;
                if(!SuperCoder::is_symbol_pivot(index))
                {
                    continue;
                }

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

        /// Set the width
        /// @param width the number of non-zero coefficients after the pivot
        void set_width(double width)
            {
                assert(width <= SuperCoder::symbols());
                m_width = width;
            }

        /// Get the width
        /// @param return the width used by the generator
        uint32_t width() const
            {
                return m_width;
            }

    private:

        /// The distribution wrapping the random generator
        boost::random::uniform_int_distribution<value_type>
        m_symbol_distribution;

        /// Generates uniform indices from the generation
        boost::random::uniform_int_distribution<uint32_t>
        m_pivot_distribution;

        /// The random generator
        boost::random::mt19937 m_random_generator;

        /// The number of non-zero values following the pivot
        uint32_t m_width;

        /// counter for number of generated coefficient vectors
        uint32_t m_generated;

    };
}
