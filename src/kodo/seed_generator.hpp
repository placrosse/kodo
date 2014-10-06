// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @ingroup NOT SURE?
    ///
    /// @brief Allows the factory to seed the encoders upon creation.

    template<class SuperCoder>
    class seed_generator : public SuperCoder
    {
    public:

        /// The seed type from the generator used


        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }

            typedef typename SuperCoder::seed_type seed_type;

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
            seed_type m_seed;
        };

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            SuperCoder::seed(the_factory.seed());
        }
    };
}
