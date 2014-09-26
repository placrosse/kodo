// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <boost/iostreams/device/mapped_file.hpp>
#include <sak/storage.hpp>

namespace kodo
{
namespace object
{
    /// @todo can we make some factory which closes the memory
    ///       mapped file even when recycling the object encoder/decoders.
    template<class SuperCoder>
    class mapped_file_sink : public SuperCoder
    {
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

            /// @param file_size Set the size in bytes of the file we
            ///        want to decode
            void set_file_size(uint32_t file_size)
            {
                assert(file_size > 0);
                m_file_size = file_size;
            }

            /// @return The size in bytes of the file we want to decode
            uint32_t file_size() const
            {
                assert(m_file_size > 0);
                return m_file_size;
            }

        protected:

            /// The size in bytes of the file we want to create an
            /// object decoder for
            uint32_t m_file_size;
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

            boost::iostreams::mapped_file_params params;
            params.path = the_factory.file_name();
            params.new_file_size = the_factory.file_size();

            m_file = boost::iostreams::mapped_file_sink(params);

            assert(m_file.is_open());

            the_factory.set_storage(
                sak::storage(m_file.data(), m_file.size()));

            SuperCoder::initialize(the_factory);
        }

        /// @copydoc layer::deinitialize(Factory&)
        template<class Factory>
        void deinitialize(Factory& the_factory)
        {
            SuperCoder::deinitialize(the_factory);

            if (m_file.is_open())
            {
                m_file.close();
            }
        }

    protected:

        /// Stores the storage for the object being encoded or decoded
        boost::iostreams::mapped_file_sink m_file;
    };
}
}
