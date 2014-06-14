// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include "../final_coder_factory_pool.hpp"
#include "../systematic_decoder.hpp"
#include "../payload_decoder.hpp"
#include "../symbol_id_decoder.hpp"
#include "../plain_symbol_id_reader.hpp"
#include "../proxy_stack.hpp"
#include "../nested_payload_recoder.hpp"
#include "../common_decoder_layers.hpp"
#include "../coefficient_storage_layers.hpp"
#include "../deep_storage_layers.hpp"
#include "../finite_field_layers.hpp"

#include "full_rlnc_recoding_stack.hpp"

namespace kodo
{


    /// @ingroup factory_layers
    /// Terminates the layered coder and contains the coder final
    /// factory. The pool factory uses a memory pool to recycle
    /// encoders/decoders, and thereby minimize memory consumption.
    template<class Codec>
    class pool_factory : public Codec::factory
    {
    public:

        /// Pointer type to the constructed coder
        typedef boost::shared_ptr<Codec> pointer;

    public:

        /// @copydoc layer::factory::factory(uint32_t,uint32_t)
        pool_factory(uint32_t max_symbols, uint32_t max_symbol_size) :
            Codec::factory(max_symbols, max_symbol_size),
            m_pool(std::bind(&pool_factory::make_coder, this))
        { }

        /// @copydoc layer::factory::build()
        pointer build()
        {
            pointer coder = m_pool.allocate();
            coder->initialize(*this);

            return coder;
        }

        /// @return A reference to the internal resource pool
        const sak::resource_pool<Codec>& pool() const
        {
            return m_pool;
        }

        /// @return A reference to the internal resource pool
        sak::resource_pool<Codec>& pool()
        {
            return m_pool;
        }

    private: // Make non-copyable

        /// Copy constructor
        pool_factory(const pool_factory&);

        /// Copy assignment
        const pool_factory& operator=(const pool_factory&);

    private:

        /// Factory function used by the resource pool to
        /// build new coders if needed.
        /// @param max_symbols The maximum symbols that are supported
        /// @param max_symbol_size The maximum size of a symbol in
        ///        bytes
        static pointer make_coder(pool_factory *factory)
        {
            assert(factory);

            pointer coder = boost::make_shared<Codec>();
            coder->construct(*factory);

            return coder;
        }

    private:

        /// Resource pool for the coders
        sak::resource_pool<Codec> m_pool;

    };




    /// @ingroup fec_stacks
    ///
    /// @brief Implementation of a complete RLNC decoder
    ///
    /// This configuration adds the following features (including those
    /// described for the encoder):
    /// - Recoding using the recoding_stack
    /// - Linear block decoder using Gauss-Jordan elimination.
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_decoder : public
        // Payload API
        nested_payload_recoder<
        proxy_stack<proxy_args<>, full_rlnc_recoding_stack,
        payload_decoder<
        // Codec Header API
        systematic_decoder<
        symbol_id_decoder<
        // Symbol ID API
        plain_symbol_id_reader<
        // Decoder API
        common_decoder_layers<TraceTag,
        // Coefficient Storage API
        coefficient_storage_layers<
        // Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Factory API
        final_coder_factory_pool<
        // Final type
        full_rlnc_decoder<Field, TraceTag>
        > > > > > > > > > > >
    {
    public:

        using factory_ = pool_factory<full_rlnc_decoder<Field, TraceTag>>;

    };
}
