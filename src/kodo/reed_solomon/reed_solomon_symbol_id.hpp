// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <map>

namespace kodo
{
    /// @ingroup symbol_id_layers
    ///
    /// @brief Base class for the Reed-Solomon symbol id reader and
    ///        writer. Contains the functionality to cache the
    ///        generator matrices.
    template<class SuperCoder>
    class reed_solomon_symbol_id : public SuperCoder
    {
    public:

        /// @copydoc layer::field_type
        typedef typename SuperCoder::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename SuperCoder::value_type value_type;

        /// The generator matrix type
        typedef typename SuperCoder::generator_matrix generator_matrix;

    public:

        /// The factory_base layer associated with this coder. Maintains
        /// the block generator needed for the encoding vectors.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory_base(max_symbols, max_symbol_size)
            { }

            /// @return A generator matrix for the number of symbols
            /// specified in the factory. We use a cache to store the
            /// generator matrix for different number of symbols.
            std::shared_ptr<generator_matrix> build_matrix()
            {
                uint32_t symbols = SuperCoder::factory_base::symbols();

                if(m_cache.find(symbols) == m_cache.end())
                {
                    m_cache[symbols] =
                        SuperCoder::factory_base::construct_matrix(symbols);
                    assert(m_cache[symbols]);
                }

                return m_cache[symbols];
            }

            /// @copydoc layer::max_id_size() const
            uint32_t max_id_size() const
            {
                return sizeof(value_type);
            }

        private:

            /// map for blocks
            std::map<uint32_t, std::shared_ptr<generator_matrix> > m_cache;
        };

    public:

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_matrix = the_factory.build_matrix();
            assert(m_matrix);

            // The row size of the generator matrix should fit
            // the expected coefficient buffer size
            assert(SuperCoder::coefficient_vector_size() ==
                   m_matrix->row_size());
        }

        /// @copydoc layer::id_size() const
        uint32_t id_size() const
        {
            return sizeof(value_type);
        }

    protected:

        /// The generator matrix
        std::shared_ptr<generator_matrix> m_matrix;

    };

}
