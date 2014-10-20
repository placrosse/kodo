// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @todo docs + tests
    ///
    /// @ingroup fulcrum
    ///
    /// @brief Keeps track of the expansion factor used in the anycode
    ///        implementation.
    template
    <
        uint32_t MaxExpansion,
        uint32_t DefaultExpansion,
        class SuperCoder
    >
    class fulcrum_info : public SuperCoder
    {
    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory_base(max_symbols, max_symbol_size),
                m_expansion(DefaultExpansion),
                m_max_inner_symbols(max_symbols + MaxExpansion)
            {
                assert(m_max_inner_symbols);
            }

            /// @return The maximum expansion supported
            uint32_t max_expansion() const
            {
                return MaxExpansion;
            }

            /// @return The expansion factor used. The expansion factor
            ///         denotes the number of additional symbols
            ///         created by the outer code.
            uint32_t expansion() const
            {
                return m_expansion;
            }

            /// Sets the number of expansion symbols
            /// @param expansion The number of expansion symbols to use
            void set_expansion(uint32_t expansion)
            {
                assert(expansion <= MaxExpansion);
                m_expansion = expansion;
            }

            /// @return the maximum number of symbols in the inner code
            uint32_t max_inner_symbols() const
            {
                return m_max_inner_symbols;
            }

        private:

            /// The number of symbols in the inner code
            uint32_t m_expansion;

            /// The total number of symbols in the inner code
            uint32_t m_max_inner_symbols;
        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_expansion = the_factory.expansion();
            m_inner_symbols = m_expansion + the_factory.symbols();
        }

        /// @return The maximum expansion supported
        uint32_t max_expansion() const
        {
            return MaxExpansion;
        }

        /// @return The expansion factor used. The expansion factor
        ///         denotes the number of additional symbols
        ///         created by the outer code.
        uint32_t expansion() const
        {
            return m_expansion;
        }

        /// @return the maximum number of symbols in the inner code
        uint32_t inner_symbols() const
        {
            return m_inner_symbols;
        }

    private:

        /// The number of symbols added by the inner code
        uint32_t m_expansion;

        /// The total number of symbols in the inner code
        uint32_t m_inner_symbols;
    };
}
