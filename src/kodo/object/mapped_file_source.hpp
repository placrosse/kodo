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
    /// @ingroup object_layers
    ///
    /// @brief Layer which memory maps a file for reading
    template<class SuperCoder>
    class mapped_file_source : public SuperCoder
    {
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
                the_factory.file_name());

            assert(m_file.is_open());

            the_factory.set_storage(sak::storage(m_file.data(), m_file.size()));

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
        boost::iostreams::mapped_file_source m_file;
    };
}
}
