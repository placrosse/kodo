// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <sak/storage.hpp>

namespace kodo
{
    /// @todo tests + docs
    ///
    /// @brief Special layer which takes all (or at least most)
    ///        function calls and forwards it to the a defined nested
    ///        stack.
    template<class SuperCoder>
    class nested_catch_all : public SuperCoder
    {
    public:

        typedef typename SuperCoder::nested_stack_type nested_stack_type;

        /// @copydoc layer::value_type
        typedef typename nested_stack_type::value_type value_type;

    public:

        /// @ingroup factory_layers
        /// Forwarding factory for the parallel nested stack
        class factory : public SuperCoder::factory
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory(uint32_t max_symbols, uint32_t max_symbol_size)
                : SuperCoder::factory(max_symbols, max_symbol_size)
            { }

            /// @copydoc layer::factory::max_symbols() const
            uint32_t max_symbols() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.max_symbols();
            }

            /// @copydoc layer::factory::max_symbol_size() const
            uint32_t max_symbol_size() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.max_symbol_size();
            }

            /// @copydoc layer::factory::max_block_size() const
            uint32_t max_block_size() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.max_block_size();
            }

            /// @copydoc layer::factory::max_header_size() const
            uint32_t max_header_size() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.max_header_size();
            }

            /// @copydoc layer::factory::max_id_size() const
            uint32_t max_id_size() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.max_id_size();
            }

            /// @copydoc layer::factory::max_payload_size() const
            uint32_t max_payload_size() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.max_payload_size();
            }

            /// @copydoc layer::factory::max_coefficient_vector_size() const
            uint32_t max_coefficient_vector_size() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.max_coefficient_vector_size();
            }

            /// @copydoc layer::factory::symbols() const;
            uint32_t symbols() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.symbols();
            }

            /// @copydoc layer::factory::symbol_size() const;
            uint32_t symbol_size() const
            {
                const auto& nested = SuperCoder::factory::nested();
                return nested.symbol_size();
            }

        };

    public:

        //------------------------------------------------------------------
        // SYMBOL STORAGE API
        //------------------------------------------------------------------

        /// @copydoc layer::copy_symbols(const sak::mutable_storage&) const
        void copy_symbols(const sak::mutable_storage &dest) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->copy_symbols(dest);
        }

        /// @copydoc layer::copy_symbol(
        ///              uint32_t,const sak::mutable_storage&) const
        void copy_symbol(uint32_t index,
                         const sak::mutable_storage &dest) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->copy_symbol(index, dest);
        }

        /// @copydoc layer::symbol(uint32_t)
        uint8_t* symbol(uint32_t index)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbol(index);
        }

        /// @copydoc layer::symbol(uint32_t) const
        const uint8_t* symbol(uint32_t index) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbol(index);
        }

        /// @copydoc layer::symbol_value(uint32_t)
        value_type* symbol_value(uint32_t index)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbol_value(index);
        }

        /// @copydoc layer::symbol_value(uint32_t) const
        const value_type* symbol_value(uint32_t index) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbol_value(index);
        }

        /// @copydoc layer::symbols() const
        uint32_t symbols() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbols();
        }

        /// @copydoc layer::symbol_size() const
        uint32_t symbol_size() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbol_size();
        }

        /// @copydoc layer::symbol_length() const
        uint32_t symbol_length() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbol_length();
        }

        /// @copydoc layer::block_size() const
        uint32_t block_size() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->block_size();
        }

        /// @copydoc layer::symbols_available() const
        uint32_t symbols_available() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbols_available();
        }

        /// @copydoc layer::symbols_initialized() const
        uint32_t symbols_initialized() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->symbols_initialized();
        }

        /// @copydoc layer::is_symbols_available() const
        bool is_symbols_available() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->is_symbols_available();
        }

        /// @copydoc layer::is_symbols_initialized() const
        bool is_symbols_initialized() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->is_symbols_initialized();
        }

        /// @copydoc layer::is_symbol_available(uint32_t) const
        bool is_symbol_available(uint32_t symbol_index) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->is_symbol_available(symbol_index);
        }

        /// @copydoc layer::is_symbol_initialized(uint32_t) const
        bool is_symbol_initialized(uint32_t symbol_index) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->is_symbol_initialized(symbol_index);
        }

        //------------------------------------------------------------------
        // COEFFICIENT STORAGE API
        //------------------------------------------------------------------

        /// @copydoc layer::coefficient_vector_size() const
        uint32_t coefficient_vector_size() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->coefficient_vector_size();
        }

        /// @copydoc layer::coefficient_vector_length() const
        uint32_t coefficient_vector_length() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->coefficient_vector_length();
        }

        /// @copydoc layer::coefficient_vector_values(uint32_t)
        value_type* coefficient_vector_values(uint32_t index)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->coefficient_vector_values(index);
        }

        /// @copydoc layer::coefficient_vector_value(uint32_t) const
        const value_type* coefficient_vector_values(uint32_t index) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->coefficient_vector_values(index);
        }

        /// @copydoc layer::coefficient_vector_data(uint32_t)
        uint8_t* coefficient_vector_data(uint32_t index)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->coefficient_vector_data(index);
        }

        /// @copydoc layer::coefficient_vector_data(uint32_t) const
        const uint8_t* coefficient_vector_data(uint32_t index) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->coefficient_vector_data(index);
        }

        //------------------------------------------------------------------
        // FINITE FIELD API
        //------------------------------------------------------------------

        /// @copydoc layer::multiply(value_type*,value_type,uint32_t)
        void multiply(value_type* symbol_dest, value_type coefficient,
                      uint32_t symbol_length)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->multiply(symbol_dest, coefficient, symbol_length);
        }

        /// @copydoc layer::multipy_add(value_type *, const value_type*,
        ///                             value_type, uint32_t)
        void multiply_add(
            value_type* symbol_dest, const value_type* symbol_src,
            value_type coefficient, uint32_t symbol_length)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->multiply_add(symbol_dest, symbol_src,
                                  coefficient, symbol_length);
        }

        /// @copydoc layer::add(value_type*, const value_type *, uint32_t)
        void add(value_type* symbol_dest, const value_type* symbol_src,
                 uint32_t symbol_length)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->add(symbol_dest, symbol_src, symbol_length);
        }

        /// @copydoc layer::multiply_subtract(value_type*,
        ///                                   const value_type*,
        ///                                   value_type, uint32_t)
        void multiply_subtract(
            value_type* symbol_dest, const value_type* symbol_src,
            value_type coefficient, uint32_t symbol_length)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->multiply_subtract(symbol_dest, symbol_src,
                                       coefficient, symbol_length);
        }

        /// @copydoc layer::subtract(
        ///              value_type*,const value_type*, uint32_t)
        void subtract(value_type* symbol_dest, const value_type* symbol_src,
                      uint32_t symbol_length)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->subtract(symbol_dest, symbol_src, symbol_length);
        }

        /// @copydoc layer::invert(value_type)
        value_type invert(value_type value)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->invert(value);
        }

        //------------------------------------------------------------------
        // CODEC API
        //------------------------------------------------------------------

        /// @copydoc layer::encode_symbol(uint8_t*, uint8_t*)
        void encode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->encode_symbol(symbol_data, coefficients);
        }

        /// @copydoc layer::encode_symbol(uint8_t*,uint32_t)
        void encode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->encode_symbol(symbol_data, symbol_index);
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data, uint8_t *coefficients)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->decode_symbol(symbol_data, coefficients);
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->decode_symbol(symbol_data, symbol_index);
        }

        /// @copydoc layer::is_complete() const
        bool is_complete() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->is_complete();
        }

        /// @copydoc layer::rank() const
        uint32_t rank() const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->rank();
        }

        /// @copydoc layer::is_symbol_pivot(uint32_t) const
        bool is_symbol_pivot(uint32_t index) const
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            return nested->is_symbol_pivot(index);
        }

        //------------------------------------------------------------------
        // COEFFICIENT GENERATOR API
        //------------------------------------------------------------------

        /// @todo add unit tests
        /// @copydoc layer::generate(uint8_t*)
        void generate(uint8_t *symbol_coefficients)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->generate(symbol_coefficients);
        }

        /// @copydoc layer::generate_partial(uint8_t*)
        void generate_partial(uint8_t *symbol_coefficients)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->generate_partial(symbol_coefficients);
        }

        /// @todo seed value -> position generator?
        /// @copydoc layer::seed(seed_type)
        void seed(uint64_t seed_value)
        {
            const auto& nested = SuperCoder::nested();
            assert(nested);
            nested->seed(seed_value);
        }

    };

}
