// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/random/mersenne_twister.hpp>           //mt11213b and mt19937
#include <boost/random/shuffle_order.hpp>              //knuth_b and kreutzer1986
#include <boost/random/taus88.hpp>                     //taus88
#include <boost/random/uniform_int_distribution.hpp>

#include <kodo/basic_factory.hpp>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @ingroup coefficient_generator_layers
    /// @brief Generates an uniform random coefficient (from the chosen
    /// Finite Field) for every symbol.
    template<class RandomNumberGenerator,
        class ValueType,
        class SuperCoder>
    class new_uniform_generator : public SuperCoder
    {

    public:

    using factory =
        basic_factory<new_uniform_generator<RandomNumberGenerator,
                          SuperCoder,
                          ValueType>>;

    public:

        /// @copydoc layer::value_type
         typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
         typedef typename SuperCoder::value_type value_type;

        /// The random generator used
         typedef  RandomNumberGenerator generator_type;

        /// @copydoc layer::seed_type
         typedef typename generator_type::result_type seed_type;

    private:
        uint32_t next_pos_value_type = 0;
        uint32_t next_pos_uint8 = 0;

    public:

        /// Constructor
        new_uniform_generator()
            : m_distribution(),
              m_value_distribution(field_type::min_value,
                                   field_type::max_value)
        { }

    public:

        class factory_base : public SuperCoder::factory_base
        {
        public:
            // @copydoc layer::factory_base::factory_base(uint32_t, uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            size = the_factory.max_coefficient_vector_size;
            if(SuperCoder::coefficient_vector_size >= 4)
            {
                vector_size_3_or_less = false;
                generate_first_ran_numbers(the_factory);
                vector_spaces_left_empty = size % sizeof(ValueType);
            }
        }

    public:
        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            if(!vector_size_3_or_less)
            {
                uint32_t array_lenght = 2 * vector_size;
                uint32_t upper_bound = size - vector_spaces_left_empty;

                for(uint32_t i = 0; i < upper_bound; ++i)
                {
                    coefficients[i * sizeof(ValueType)] =
                        random_numbers_value_type[(next_pos_value_type + i) % array_lenght];

                }

                //After each coefficient vector has been
                //assigned values the first value used for the vector is replaced by
                //a new random number. The next vector will used the second used value
                //as its first value.

                //printf("Next coeffients\n\n");
                random_numbers_value_type[next_pos_value_type] =
                    m_value_distribution(m_random_generator);
                // printf("new number is %u\n", random_numbers[pos_next_ran_number]);
                next_pos_value_type = (next_pos_value_type + 1) % array_lenght;

                if(vector_spaces_left_empty)
                {
                    uint32_t array_lenght = 2 * vector_spaces_left_empty;
                    for(uint32_t i = upper_bound - vector_spaces_left_empty;
                        i < upper_bound; ++i)
                    {
                        coefficients[i] =
                            random_numbers_uint8[(next_pos_uint8 + i) % array_lenght];

                    }
                }
            }
            else
            {

            }
        }

        /// @copydoc layer::generate(uint8_t*)
        void generate_partial(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            // Since we will not set all coefficients we should ensure
            // that the non specified ones are zero
            std::fill_n(coefficients, SuperCoder::coefficient_vector_size(), 0);

            value_type *c = reinterpret_cast<value_type*>(coefficients);

            uint32_t symbols = SuperCoder::symbols();

            for(uint32_t i = 0; i < symbols; ++i)
            {
                if(!SuperCoder::can_generate(i))
                {
                    continue;
                }

                value_type coefficient =
                    m_value_distribution(m_random_generator);

                fifi::set_value<field_type>(c, i, coefficient);
            }
        }

        /// @copydoc layer::seed(seed_type)
        void seed(seed_type seed_value)
        {
            m_random_generator.seed(seed_value);
        }

    private:
        //Shall call max_symbols, but SuperCoder::max_symbols does not work.
        //For the test is is set to 16.
        std::vector<ValueType> random_numbers_value_type;
        std::vector<uint8_t> random_numbers_uint8;

        //Generates 2 * MAX_SYMBOL_SIZE of random numbers. These are the numbers
        //that will be used as coefficents.
        template<class Factory>
        void generate_first_random_numbers(Factory& the_factory)
        {

            // The size of the vectors containing random numbers is defined.
            vector_size =
                ((size - vector_spaces_left_empty) / sizeof(ValueType)) * 2;
            random_numbers_value_type.resize(vector_size);

            // The two vectors are filled with random numbers
            for(uint32_t i = 0; i < (random_numbers_value_type.size()); ++i)
            {
                random_numbers_value_type[i] =
                    m_value_distribution(m_random_generator);
                //printf("%u\n", random_numbers[i]);
            }

            if(vector_spaces_left_empty)
            {
                random_numbers_uint8.resize(vector_spaces_left_empty * 2);

                for(uint32_t i = 0; i < random_numbers_uint8.size(); ++i)
                {
                    random_numbers_uint8[i] =
                        m_distribution(m_random_generator);
                }
            }
        }

    private:

        // Boolean set after the coefficient vector size
        bool vector_size_3_or_less = true;

        uint32_t vector_spaces_left_empty;
        uint32_t size;
        uint32_t vector_size;


    private:
        /// The type of the uint8_t distribution
        typedef boost::random::uniform_int_distribution<uint8_t>
        uint8_t_distribution;

        /// Distribution that generates random bytes
        uint8_t_distribution m_distribution;

        /// The type of the value_type distribution
        typedef boost::random::uniform_int_distribution<ValueType>
        value_type_distribution;

        /// Distribution that generates random values from a finite field
        value_type_distribution m_value_distribution;

        /// The random generator
        RandomNumberGenerator m_random_generator;

    };
}
