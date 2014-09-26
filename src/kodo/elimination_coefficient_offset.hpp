// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{

    /// @ingroup fullcrum
    ///
    /// @brief Building block for creating an "elimination decoder".
    ///        This layer keeps track of the offset which is added when
    ///        accessing the coefficients in the coefficients vector.
    template<class SuperCoder>
    class elimination_coefficient_offset : public SuperCoder
    {
    public:

        /// @ingroup factory_base_layers
        /// The factory_base layer associated with this coder.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory_base(max_symbols, max_symbol_size),
                m_offset(0)
            { }

            /// @return The number of coefficients that we will skip when
            ///         inspecting the coding coefficients.
            uint32_t elimination_offset() const
            {
                return m_offset;
            }

            /// Sets elimination offset i.e. how many coefficients
            /// should be skipped when iterating through the
            /// coefficient vector
            /// @param offset The offset in coding coefficients
            void set_elimination_offset(uint32_t offset)
            {
                assert(offset < SuperCoder::factory_base::max_symbols());
                m_offset = offset;
            }

        private:

            /// The offset to be used
            uint32_t m_offset;

        };

    public:

        /// Constructor
        elimination_coefficient_offset()
            : m_offset(0)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_offset = the_factory.elimination_offset();

            // The offset and the symbols that we actually plan to decode
            // cannot supersede the maximum symbols which can be stored
            // in the decoder.
            assert(the_factory.elimination_offset() + the_factory.symbols()
                   <= the_factory.max_symbols());
        }

        /// @copydoc factory::elimination_offset() const
        uint32_t elimination_offset() const
        {
            return m_offset;
        }

    private:

        /// The offset to be used
        uint32_t m_offset;

    };

}
