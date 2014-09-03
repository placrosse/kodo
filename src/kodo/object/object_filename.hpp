// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <string>

namespace kodo
{
namespace object
{
    /// @todo test + docs
    template<class SuperCoder>
    class object_filename : public SuperCoder
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
            SuperCoder::initialize(the_factory);
            m_filename = the_factory.filename();
        }

        const std::string& filename() const
        {
            return m_filename;
        }


    protected:

        /// The filename of the object
        std::string m_filename;
    };
}
}
