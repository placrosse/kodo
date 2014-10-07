// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

namespace kodo
{
    /// @ingroup coefficient_generator_layers
    ///
    /// @brief Allows the factory to seed the encoders upon creation.

    template<class SuperCoder>
    class seed_generator : public SuperCoder
    {
    public:


        /// The seed type from the used generator
        using seed_type = typename SuperCoder::seed_type;

        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }


            /// Sets a seed to be used for all encoders created with this
            /// factory
            /// @param seed The seed to use
            void set_seed(seed_type seed)
            {
                m_seed = seed;
            }

            /// @return The seed
            seed_type seed() const
            {
                return m_seed;
            }

        private:

            /// The seed used for all encoders created by this factory.
            seed_type m_seed;
        };

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            // The reason why this is done here, and not in the construct
            // function, is that after seeding the factory, the caller is likely
            // to expect that the encoders behave in the same way, regardless of
            // whether they have been reconstructed one or not.
            SuperCoder::seed(the_factory.seed());
        }
    };
}
