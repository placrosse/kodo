// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <sak/storage.hpp>

namespace kodo
{
    /// @brief Special layer used to create parallel stacks with
    ///       some shared layers.
    ///
    /// The proxy_layer takes two template arguments the FinalType and
    /// the MainStack.
    ///
    /// The FinalType is the type of the actual stack containing the
    /// proxy_layer. This is similar to the argument passed to the
    /// factory layers in the standard stacks. The proxy_layer needs
    /// this information since it also acts as a factory for the proxy
    /// stack.
    ///
    /// The MainStack is the type of the stack where the proxy_layer
    /// will forward function calls that are not handled directly in
    /// the proxy stack.
    ///
    /// The proxy_layer implements most of the API seen in the
    /// different layers. This means that if a particiular function is
    /// not implement in the stack containing the proxy_layer the
    /// proxy_layer will make sure that the function call is forwarded
    /// to the main stack.
    template<class FinalType, class MainStack>
    class proxy_layer
    {
    public:

        /// @copydoc layer::field_type
        typedef typename MainStack::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename MainStack::value_type value_type;

        /// Pointer type to the constructed coder
        typedef boost::shared_ptr<FinalType> pointer;

        /// The type of the main stack
        typedef MainStack main_stack_type;

        /// The type of the main stack
        typedef typename main_stack_type::factory_base main_factory_type;

    public:

        /// @ingroup factory_base_layers
        ///
        /// Forwarding factory_base for the parallel proxy stack
        class factory_base
        {
        public:

            /// The factory_base type
            typedef typename FinalType::factory_base factory_type;

        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size)
                : m_main_factory(0),
                  m_main_stack(0)
            {
                (void) max_symbols;
                (void) max_symbol_size;
            }

            /// Sets the pointer to the main stack factory
            ///
            /// @param main_factory The stack where calls should be
            ///        forwarded.
            void set_main_factory(main_factory_type* main_factory)
            {
                // Currently we have added the asserts in such a way
                // that the factory pointer can only be specified once
                assert(main_factory != 0);
                assert(m_main_factory == 0);

                m_main_factory = main_factory;
            }

            /// @return a pointer to the main factory
            const main_factory_type* main_factory() const
            {
                assert(m_main_factory);
                return m_main_factory;
            }

            /// @return a pointer to the main factory
            main_factory_type* main_factory()
            {
                assert(m_main_factory);
                return m_main_factory;
            }

            /// Sets the pointer to the main stack. If a function call
            /// "falls" through to the proxy_layer this is the stack
            /// that we will forward the call to.
            ///
            /// @param stack The pointer to the main stack
            void set_main_stack(main_stack_type* main_stack)
            {
                assert(main_stack != 0);
                m_main_stack = main_stack;
            }

            /// @return a pointer to the main stack
            const main_stack_type* main_stack() const
            {
                assert(m_main_stack);
                return m_main_stack;
            }

            /// @return a pointer to the main stack
            main_stack_type* main_stack()
            {
                assert(m_main_stack);
                return m_main_stack;
            }

            /// @copydoc factory::build()
            pointer build()
            {
                assert(m_main_factory != 0);
                assert(m_main_stack != 0);

                pointer coder = boost::make_shared<FinalType>();

                factory_type *this_factory =
                    static_cast<factory_type*>(this);

                coder->construct(*this_factory);
                coder->initialize(*this_factory);

                return coder;
            }

            /// @copydoc layer::factory_base::max_symbols() const
            uint32_t max_symbols() const
            {
                assert(m_main_factory);
                return m_main_factory->max_symbols();
            }

            /// @copydoc layer::factory_base::max_symbol_size() const
            uint32_t max_symbol_size() const
            {
                assert(m_main_factory);
                return m_main_factory->max_symbol_size();
            }

            /// @copydoc layer::factory_base::max_block_size() const
            uint32_t max_block_size() const
            {
                assert(m_main_factory);
                return m_main_factory->max_block_size();
            }

            /// @copydoc layer::factory_base::max_header_size() const
            uint32_t max_header_size() const
            {
                assert(m_main_factory);
                return m_main_factory->max_header_size();
            }

            /// @copydoc layer::factory_base::max_id_size() const
            uint32_t max_id_size() const
            {
                assert(m_main_factory);
                return m_main_factory->max_id_size();
            }

            /// @copydoc layer::factory_base::max_payload_size() const
            uint32_t max_payload_size() const
            {
                assert(m_main_factory);
                return m_main_factory->max_payload_size();
            }

            /// @copydoc layer::factory_base::max_coefficient_vector_size() const
            uint32_t max_coefficient_vector_size() const
            {
                assert(m_main_factory);
                return m_main_factory->max_coefficient_vector_size();
            }

            /// @copydoc layer::factory_base::symbols() const;
            uint32_t symbols() const
            {
                assert(m_main_factory);
                return m_main_factory->symbols();
            }

            /// @copydoc layer::factory_base::symbol_size() const;
            uint32_t symbol_size() const
            {
                assert(m_main_factory);
                return m_main_factory->symbol_size();
            }

        private:

            /// Pointer to the main stack's factory
            main_factory_type* m_main_factory;

            /// Pointer to the main stack used during building a stack
            main_stack_type* m_main_stack;

        };

    public:

        /// Constructor
        proxy_layer()
            : m_main_stack(0)
        { }

        /// @copydoc layer::construct(Factory&)
        template<class Factory>
        void construct(Factory &the_factory)
        {
            (void) the_factory;
        }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory &the_factory)
        {
            m_main_stack = the_factory.main_stack();
            assert(m_main_stack);
        }

        /// @return a pointer to the main proxy stack
        const main_stack_type* main_stack() const
        {
            assert(m_main_stack);
            return m_main_stack;
        }

        /// @return a pointer to the main proxy stack
        main_stack_type* main_stack()
        {
            assert(m_main_stack);
            return m_main_stack;
        }

        //------------------------------------------------------------------
        // SYMBOL STORAGE API
        //------------------------------------------------------------------

        /// @copydoc layer::copy_symbols(const sak::mutable_storage&) const
        void copy_symbols(const sak::mutable_storage &dest) const
        {
            assert(m_main_stack);
            m_main_stack->copy_symbols(dest);
        }

        /// @copydoc layer::copy_symbol(
        ///              uint32_t,const sak::mutable_storage&) const
        void copy_symbol(uint32_t index,
                         const sak::mutable_storage &dest) const
        {
            assert(m_main_stack);
            m_main_stack->copy_symbol(index, dest);
        }

        /// @copydoc layer::symbol(uint32_t)
        uint8_t* symbol(uint32_t index)
        {
            assert(m_main_stack);
            return m_main_stack->symbol(index);
        }

        /// @copydoc layer::symbol(uint32_t) const
        const uint8_t* symbol(uint32_t index) const
        {
            assert(m_main_stack);
            return m_main_stack->symbol(index);
        }

        /// @copydoc layer::symbol_value(uint32_t)
        value_type* symbol_value(uint32_t index)
        {
            assert(m_main_stack);
            return m_main_stack->symbol_value(index);
        }

        /// @copydoc layer::symbol_value(uint32_t) const
        const value_type* symbol_value(uint32_t index) const
        {
            assert(m_main_stack);
            return m_main_stack->symbol_value(index);
        }

        /// @copydoc layer::symbols() const
        uint32_t symbols() const
        {
            assert(m_main_stack);
            return m_main_stack->symbols();
        }

        /// @copydoc layer::symbol_size() const
        uint32_t symbol_size() const
        {
            assert(m_main_stack);
            return m_main_stack->symbol_size();
        }

        /// @copydoc layer::symbol_length() const
        uint32_t symbol_length() const
        {
            assert(m_main_stack);
            return m_main_stack->symbol_length();
        }

        /// @copydoc layer::block_size() const
        uint32_t block_size() const
        {
            assert(m_main_stack);
            return m_main_stack->block_size();
        }

        /// @copydoc layer::symbols_available() const
        uint32_t symbols_available() const
        {
            assert(m_main_stack);
            return m_main_stack->symbols_available();
        }

        /// @copydoc layer::symbols_initialized() const
        uint32_t symbols_initialized() const
        {
            assert(m_main_stack);
            return m_main_stack->symbols_initialized();
        }

        /// @copydoc layer::is_symbols_available() const
        bool is_symbols_available() const
        {
            assert(m_main_stack);
            return m_main_stack->is_symbols_available();
        }

        /// @copydoc layer::is_symbols_initialized() const
        bool is_symbols_initialized() const
        {
            assert(m_main_stack);
            return m_main_stack->is_symbols_initialized();
        }

        /// @copydoc layer::is_symbol_available(uint32_t) const
        bool is_symbol_available(uint32_t symbol_index) const
        {
            assert(m_main_stack);
            return m_main_stack->is_symbol_available(symbol_index);
        }

        /// @copydoc layer::is_symbol_initialized(uint32_t) const
        bool is_symbol_initialized(uint32_t symbol_index) const
        {
            assert(m_main_stack);
            return m_main_stack->is_symbol_initialized(symbol_index);
        }

        //------------------------------------------------------------------
        // COEFFICIENT STORAGE API
        //------------------------------------------------------------------

        /// @copydoc layer::coefficient_vector_size() const
        uint32_t coefficient_vector_size() const
        {
            assert(m_main_stack);
            return m_main_stack->coefficient_vector_size();
        }

        /// @copydoc layer::coefficient_vector_length() const
        uint32_t coefficient_vector_length() const
        {
            assert(m_main_stack);
            return m_main_stack->coefficient_vector_length();
        }

        /// @copydoc layer::coefficient_vector_values(uint32_t)
        value_type* coefficient_vector_values(uint32_t index)
        {
            assert(m_main_stack);
            return m_main_stack->coefficient_vector_values(index);
        }

        /// @copydoc layer::coefficient_vector_value(uint32_t) const
        const value_type* coefficient_vector_values(uint32_t index) const
        {
            assert(m_main_stack);
            return m_main_stack->coefficient_vector_values(index);
        }

        /// @copydoc layer::coefficient_vector_data(uint32_t)
        uint8_t* coefficient_vector_data(uint32_t index)
        {
            assert(m_main_stack);
            return m_main_stack->coefficient_vector_data(index);
        }

        /// @copydoc layer::coefficient_vector_data(uint32_t) const
        const uint8_t* coefficient_vector_data(uint32_t index) const
        {
            assert(m_main_stack);
            return m_main_stack->coefficient_vector_data(index);
        }

        //------------------------------------------------------------------
        // FINITE FIELD API
        //------------------------------------------------------------------

        /// @copydoc layer::multiply(value_type*,value_type,uint32_t)
        void multiply(value_type *symbol_dest, value_type coefficient,
                      uint32_t symbol_length)
        {
            assert(m_main_stack);
            m_main_stack->multiply(symbol_dest, coefficient, symbol_length);
        }

        /// @copydoc layer::multipy_add(value_type *, const value_type*,
        ///                             value_type, uint32_t)
        void multiply_add(
            value_type *symbol_dest, const value_type *symbol_src,
            value_type coefficient, uint32_t symbol_length)
        {
            assert(m_main_stack);
            m_main_stack->multiply_add(symbol_dest, symbol_src,
                                  coefficient, symbol_length);
        }

        /// @copydoc layer::add(value_type*, const value_type *, uint32_t)
        void add(value_type *symbol_dest, const value_type *symbol_src,
                 uint32_t symbol_length)
        {
            assert(m_main_stack);
            m_main_stack->add(symbol_dest, symbol_src, symbol_length);
        }

        /// @copydoc layer::multiply_subtract(value_type*,
        ///                                   const value_type*,
        ///                                   value_type, uint32_t)
        void multiply_subtract(
            value_type *symbol_dest, const value_type *symbol_src,
            value_type coefficient, uint32_t symbol_length)
        {
            assert(m_main_stack);
            m_main_stack->multiply_subtract(symbol_dest, symbol_src,
                                       coefficient, symbol_length);
        }

        /// @copydoc layer::subtract(
        ///              value_type*,const value_type*, uint32_t)
        void subtract(value_type *symbol_dest, const value_type *symbol_src,
                      uint32_t symbol_length)
        {
            assert(m_main_stack);
            m_main_stack->subtract(symbol_dest, symbol_src, symbol_length);
        }

        /// @copydoc layer::invert(value_type)
        value_type invert(value_type value)
        {
            assert(m_main_stack);
            return m_main_stack->invert(value);
        }

        //------------------------------------------------------------------
        // CODEC API
        //------------------------------------------------------------------

        /// @copydoc layer::encode_symbol(uint8_t*, uint8_t*)
        void encode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            assert(m_main_stack);
            m_main_stack->encode_symbol(symbol_data, coefficients);
        }

        /// @copydoc layer::encode_symbol(uint8_t*,uint32_t)
        void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            assert(m_main_stack);
            m_main_stack->encode_symbol(symbol_data, symbol_index);
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            assert(m_main_stack);
            m_main_stack->decode_symbol(symbol_data, coefficients);
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            assert(m_main_stack);
            m_main_stack->decode_symbol(symbol_data, symbol_index);
        }

        /// @copydoc layer::is_complete() const
        bool is_complete() const
        {
            assert(m_main_stack);
            return m_main_stack->is_complete();
        }

        /// @copydoc layer::rank() const
        uint32_t rank() const
        {
            assert(m_main_stack);
            return m_main_stack->rank();
        }

        /// @copydoc layer::is_symbol_pivot(uint32_t) const
        bool is_symbol_pivot(uint32_t index) const
        {
            assert(m_main_stack);
            return m_main_stack->is_symbol_pivot(index);
        }

        /// @copydoc layer::is_symbol_uncoded(uint32_t) const
        bool is_symbol_uncoded(uint32_t index) const
        {
            assert(m_main_stack);
            return m_main_stack->is_symbol_uncoded(index);
        }

        /// @copydoc layer::symbols_uncoded() const
        uint32_t symbols_uncoded() const
        {
            assert(m_main_stack);
            return m_main_stack->symbols_uncoded();
        }

    protected:

        /// Pointer to the main stack
        main_stack_type *m_main_stack;

    };

}
