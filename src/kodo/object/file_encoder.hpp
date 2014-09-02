// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/iostreams/device/mapped_file.hpp>
#include <sak/storage.hpp>

#include "stack_factory.hpp"
#include "rfc5052_partitioning.hpp"
#include "object_storage.hpp"

#include "../rebind_factory.hpp"
#include "../final_layer.hpp"

namespace kodo
{
namespace object
{

    /// @todo can we make some factory which closes the memory
    ///       mapped file even when recycling the object encoder/decoders.
    template<class SuperCoder>
    class mapped_file_source : public SuperCoder
    {
    public:

    public:

        /// @ingroup factory_base_layers
        ///
        /// @brief The factory layer allows a user to specify the
        ///        storage that should be used for the
        ///        encoding/decoding.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory_base::factory_base(uint32_t,uint32_t)
            factory_base(uint32_t symbols, uint32_t symbol_size)
                : SuperCoder::factory_base(symbols, symbol_size)
            { }

            void set_filename(const std::string &filename)
            {
                assert(!filename.empty());
                m_filename = filename;
            }

            const std::string& filename() const
            {
                assert(!m_filename.empty());
                return m_filename;
            }

        protected:

            /// The filename of the source
            std::string m_filename;
        };

    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            // Here we do something a bit unconventional normally we
            // would always call initialize before doing
            // anything. However in order for use to re-use the
            // object_storage layer we open the memory mapped file and
            // initializes the factory with the object storage so that
            // the layer will be able to map the files content
            // correctly to the encoders

            m_file = boost::iostreams::mapped_file_source(
                the_factory.filename());

            assert(m_file.is_open());

            the_factory.set_storage(sak::storage(m_file.data(), m_file.size()));

            SuperCoder::initialize(the_factory);
        }

    protected:

        /// Stores the storage for the object being encoded or decoded
        boost::iostreams::mapped_file_source m_file;
    };

    /// @todo finish docs
    template<class Stack>
    class file_encoder : public
        mapped_file_source<
        object_storage<
        stack_factory<Stack,
        rfc5052_partitioning<
        final_layer> > > >
    {
    public:

        /// We will use the same factory as the stack
        using factory = rebind_factory<Stack, file_encoder>;
    };
}
}
