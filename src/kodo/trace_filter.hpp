// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <memory>

namespace kodo
{
    /// @todo docs+tests
    struct no_filter
    {
        bool operator()(const std::string& zone) const
        {
            (void) zone;
            return true;
        }
    };

    class trace_filter
    {
    public:

        trace_filter()
            : m_filter(new impl<no_filter>())
        { }

        template<class T>
        trace_filter(const T& t)
            : m_filter(new impl<T>(t))
        { }

        bool operator()(const std::string& zone) const
        {
            return m_filter->filter(zone);
        }

        struct model
        {
            virtual bool filter(const std::string&) const = 0;
        };

        template<class T>
        struct impl : public model
        {
            impl(const T& t)
                : m_t(t)
            { }

            impl()
            { }

            bool filter(const std::string& zone) const
            {
                return m_t(zone);
            }

            T m_t;
        };

        std::shared_ptr<model> m_filter;
    };
}
