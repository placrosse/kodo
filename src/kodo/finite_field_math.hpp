// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <type_traits>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @ingroup finite_field_layers
    /// @brief Basic layer performing common finite field operation
    template<class FieldImpl, class SuperCoder>
    class finite_field_math : public SuperCoder
    {
    public:

        /// The field type used
        typedef typename SuperCoder::field_type field_type;

        /// The value type
        typedef typename SuperCoder::value_type value_type;

        /// The finite field implementation
        typedef FieldImpl field_impl;

        /// Pointer to the finite field implementation
        typedef boost::shared_ptr<field_impl> field_pointer;

        /// Pointer to coder produced by the factories
        typedef typename SuperCoder::pointer pointer;

    private:

        /// The field type of the finite field implementation
        typedef typename field_impl::field_type impl_field_type;

        /// Check that the fields match
        static_assert(std::is_same<impl_field_type, field_type>::value,
                      "Chosen field must match.");

    public:

        /// @ingroup factory_layers
        /// The factory layer associated with this coder. We create
        /// an instance of the used field and share this with all coders
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory_base(max_symbols, max_symbol_size)
            {
                m_field = boost::make_shared<field_impl>();
            }

        private:

            /// Give the layer access
            friend class finite_field_math;

            /// @return The field implementation used.
            field_pointer field()
            {
                return m_field;
            }

        protected:

            /// The field implementation
            field_pointer m_field;
        };

    public:

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            SuperCoder::construct(the_factory);

            // The maximum symbol length needed for the temp symbol.
            // We expect this will be one value per symbol
            // (for the encoding vector) or the number of symbols
            // needed for the actual data.

            uint32_t data_symbol_length =
                fifi::size_to_length<field_type>(
                    the_factory.max_symbol_size());

            uint32_t max_symbol_length =
                std::max(the_factory.max_symbols(), data_symbol_length);

            assert(max_symbol_length > 0);

            m_field = the_factory.field();
        }

        /// @copydoc layer::multiply(value_type*,value_type,uint32_t)
        void multiply(value_type *symbol_dest, value_type coefficient,
                      uint32_t symbol_length)
        {
            assert(m_field);
            assert(symbol_dest != 0);
            assert(symbol_length > 0);

            coefficient = fifi::pack_constant<field_type>(coefficient);

            m_field->region_multiply_constant(symbol_dest, coefficient,
                symbol_length);
        }

        /// @copydoc layer::multipy_add(value_type *, const value_type*,
        ///                             value_type, uint32_t)
        void multiply_add(value_type *symbol_dest,
                          const value_type *symbol_src,
                          value_type coefficient, uint32_t symbol_length)
        {
            assert(m_field);
            assert(symbol_dest != 0);
            assert(symbol_src != 0);
            assert(symbol_length > 0);

            coefficient = fifi::pack_constant<field_type>(coefficient);

            m_field->region_multiply_add(symbol_dest,
                               symbol_src, coefficient, symbol_length);
        }

        /// @copydoc layer::add(value_type*, const value_type *, uint32_t)
        void add(value_type *symbol_dest, const value_type *symbol_src,
                 uint32_t symbol_length)
        {
            assert(m_field);
            assert(symbol_dest != 0);
            assert(symbol_src  != 0);
            assert(symbol_length > 0);

            m_field->region_add(symbol_dest, symbol_src, symbol_length);
        }

        /// @copydoc layer::multiply_subtract(value_type*, const value_type*,
        ///                                   value_type, uint32_t)
        void multiply_subtract(value_type *symbol_dest,
                               const value_type *symbol_src,
                               value_type coefficient,
                               uint32_t symbol_length)
        {
            assert(m_field);
            assert(symbol_dest != 0);
            assert(symbol_src  != 0);
            assert(symbol_length > 0);
            assert(symbol_dest != symbol_src);

            coefficient = fifi::pack_constant<field_type>(coefficient);

            m_field->region_multiply_subtract(symbol_dest,
                symbol_src, coefficient, symbol_length);
        }

        /// @copydoc layer::subtract(value_type*,const value_type*, uint32_t)
        void subtract(value_type *symbol_dest, const value_type *symbol_src,
                      uint32_t symbol_length)
        {
            assert(m_field);
            assert(symbol_dest != 0);
            assert(symbol_src  != 0);
            assert(symbol_length > 0);

            m_field->region_subtract(symbol_dest, symbol_src, symbol_length);
        }

        /// @copydoc layer::invert(value_type)
        value_type invert(value_type value)
        {
            assert(m_field);
            return m_field->invert( value );
        }

    private:

        /// The selected field
        field_pointer m_field;
    };

}
