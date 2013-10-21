// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <boost/dynamic_bitset.hpp>

namespace kodo
{

    /// @ingroup symbol_storage_layers
    /// @brief Implements function to retrieve symbols currently available
    ///        in the symbol storage.
    ///
    /// This kind of functionality is needed when an encoder should be
    /// able to support encoding before all symbols are ready. The
    /// coefficient generator layers uses this information to create
    /// partial coefficient vectors if needed.
    template<class SuperCoder>
    class storage_status_vector : public SuperCoder
    {

    protected:

        typedef uint8_t status_block;
        typedef boost::dynamic_bitset<status_block> status_vector;

    public:

        /// @copdydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_status.resize(the_factory.max_symbols(), false);
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_status.reset();
        }

        /// @copydoc layer::set_symbols(const sak::const_storage&)
        void set_symbols(const sak::const_storage &symbol_storage)
        {
            m_status.set();
            SuperCoder::set_symbols(symbol_storage);
        }

        /// @copydoc layer::set_symbol(uint32_t, const sak::const_storage&)
        void set_symbol(uint32_t index, const sak::const_storage &symbol)
        {
            assert(index < SuperCoder::symbols());

            m_status[index] = true;
            SuperCoder::set_symbol(index, symbol);
        }

        /// @copydoc layer::copy_into_symbols(const sak::const_storage&)
        void copy_into_symbols(const sak::const_storage &src)
        {
            m_status.set();
            SuperCoder::copy_into_symbols(src);
        }

        /// @copydoc layer::swap_symbols(std::vector<uint8_t> &)
        void swap_symbols(std::vector<uint8_t> &symbols)
        {
            m_status.set();
            SuperCoder::swap_symbols(symbols);
        }

        const status_vector& storage_status() const
        {
            return m_status;
        }

    private:

        status_vector m_status;

    };

}
