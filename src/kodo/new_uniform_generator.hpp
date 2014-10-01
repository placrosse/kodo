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
        typename ValueType,
        class SuperCoder>
    class new_uniform_generator : public SuperCoder
    {

    public:

     // using factory =
         // basic_factory<new_uniform_generator<RandomNumberGenerator,
                          // ValueType,
                          // SuperCoder>>;

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
        void* temp_ptr;
        ValueType* castet_coefficient_ptr;

    public:

        /// Constructor
        new_uniform_generator()
            : m_distribution(),
              m_super_value_distribution(field_type::min_value,
                                   field_type::max_value)
        { }


    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            size = the_factory.max_coefficient_vector_size();

            if(SuperCoder::coefficient_vector_size() >= sizeof(ValueType))
            {
                vector_size_less_value_type_size = false;
                if(size % sizeof(ValueType))
                {
                    uint8s.resize((size % sizeof(ValueType)) * 2);
                }
            }

            generate_first_random_numbers();
        }

    public:

        void generate(uint8_t *coefficients)
        {
            assert(coefficients != 0);

            if(!vector_size_less_value_type_size)
            {

                temp_ptr = static_cast<void*>(coefficients);
                castet_coefficient_ptr = static_cast<ValueType*>(temp_ptr);

                for(uint32_t i = 0; i < upper_bound; ++i)
                {
                    castet_coefficient_ptr[i] =
                        value_types[(next_pos_value_type + i) % array_lenght];
                }

                value_types[next_pos_value_type] =
                    m_value_distribution(m_random_generator);

                next_pos_value_type = (next_pos_value_type + 1) % array_lenght;

                if(uint8s.size())
                {
                    // uint32_t k =
                        // upper_bound * sizeof(ValueType) + uint8s.size();
                    uint32_t starting_point = upper_bound * sizeof(ValueType);

                    for(uint32_t i = starting_point; i < size; ++i)
                    {
                        coefficients[i] =
                            uint8s[(next_pos_uint8 + i) % uint8s.size()];

                        uint8s[next_pos_uint8] =
                            m_distribution(m_random_generator);
                        next_pos_uint8 = (next_pos_uint8 + 1) % uint8s.size();
                    }
                }
            }
            else
            {
                for(uint32_t i = 0; i < size; ++i)
                {
                    coefficients[i] =
                        uint8s[(next_pos_uint8 + i) % array_lenght];

                }
                uint8s[next_pos_uint8] =
                    m_distribution(m_random_generator);

                next_pos_uint8 = (next_pos_uint8 + 1) % array_lenght;
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
                    m_super_value_distribution(m_random_generator);

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
        std::vector<ValueType> value_types;
        std::vector<uint8_t> uint8s;

        //Generates 2 * MAX_SYMBOL_SIZE of random numbers. These are the numbers
        //that will be used as coefficents.
        void generate_first_random_numbers()
        {

            // The size of the vectors containing random numbers is defined.
            vector_size =
                ((size - uint8s.size()) / sizeof(ValueType)) * 2;
            value_types.resize(vector_size);

            array_lenght =  vector_size;

            upper_bound = (size - uint8s.size()) / sizeof(ValueType);



            // The two vectors are filled with random numbers
            for(uint32_t i = 0; i < array_lenght; ++i)
            {
                value_types[i] =
                    m_value_distribution(m_random_generator);
            }

            if(uint8s.size())
            {

                for(uint32_t i = 0; i < uint8s.size(); ++i)
                {
                    uint8s[i] =
                        m_distribution(m_random_generator);
                }
            }
            else if(vector_size_less_value_type_size)
            {
                // assert(0);
                uint8s.resize(SuperCoder::coefficient_vector_size() * 2);
                array_lenght = uint8s.size();

                for(uint32_t i = 0; i < array_lenght; ++i)
                {
                    uint8s[i] =
                        m_distribution(m_random_generator);
                }
            }
        }

    private:

        // Boolean set after the coefficient vector size
        bool vector_size_less_value_type_size = true;

        uint32_t array_lenght;
        uint32_t upper_bound;
        // uint32_t surplus_coef_spaces = 0;
        // uint32_t vector_spaces_left_empty = 0;
        uint32_t size;
        uint32_t vector_size;


    private:
        /// The type of the uint8_t distribution
        typedef boost::random::uniform_int_distribution<uint8_t>
        uint8_t_distribution;

        /// Distribution that generates random bytes
        uint8_t_distribution m_distribution;

        /// The type of the value_type distribution
        typedef boost::random::uniform_int_distribution<value_type>
        SuperCoder_value_type_distribution;

        /// Distribution that generates random values from a finite field
        SuperCoder_value_type_distribution m_super_value_distribution;

        /// The type of the value_type distribution
        typedef boost::random::uniform_int_distribution<ValueType>
        value_type_distribution;

        /// Distribution that generates random values from a finite field
        value_type_distribution m_value_distribution;

        /// The random generator
        RandomNumberGenerator m_random_generator;

    };
}
