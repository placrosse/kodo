// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

namespace stub
{
    template<class R>
    class return_handler
    {
    public:

        return_handler()
            : m_position(0)
        {}

        void set_return(const R& value)
        {
            m_returns.push_back(value);
        }

        void set_returns(const std::initializer_list<R>& values)
        {
            m_returns.insert(m_returns.end(),
                             values.begin(), values.end());
        }

        R operator()() const
        {
            assert(m_position < m_returns.size());

            R value = m_returns[m_position];
            ++m_position;

            return value;
        }

        mutable uint32_t m_position;
        std::vector<R> m_returns;
    };

    template<>
    class return_handler<void>
    {
    public:

        void operator()() const
        {
        }
    };

    template<class... Args>
    class call_handler
    {
    public:

        using arguments = std::tuple<Args...>;

        void add_call(Args... args) const
        {
            m_calls.emplace_back(args...);
        }

        bool called_once_with(Args... args) const
        {
            auto p = [](const arguments& a, const arguments& b) -> bool
                { return a == b; };

            return called_once_with(args..., p);
        }


        template<class BinaryPredicate>
        bool called_once_with(Args... args,
                              const BinaryPredicate& predicate) const
        {
            if (m_calls.size() != 1U)
            {
                return false;
            }

            return called_with(args..., predicate);
        }

        bool called_with(Args... args) const
        {
            auto p = [](const arguments& a, const arguments& b) -> bool
                { return a == b; };

            return called_with(args..., p);
        }

        template<class BinaryPredicate>
        bool called_with(Args... args,
                         const BinaryPredicate& predicate) const
        {
            assert(m_calls.size() > 0);

            // We know there is one element in the list
            const auto& stored_args = m_calls.back();
            const auto& reference_args = std::make_tuple(args...);

            return predicate(stored_args,reference_args);
        }

        mutable std::vector<arguments> m_calls;
    };

    template<typename T> class call;

    template<typename R, typename... Args> class call<R (Args...)>
    {
    public:

        R operator()(Args... args) const
        {
            m_call_handler.add_call(args...);
            return m_return_handler();
        }

        template<class T>
        void set_return(const T& return_value)
        {
            m_return_handler.set_return(return_value);
        }

        template<class T>
        void set_returns(const std::initializer_list<T> &returns)
        {
            m_return_handler.set_returns(returns);
        }

        bool called_once_with(Args... args) const
        {
            return m_call_handler.called_once_with(args...);
        }

        template<class BinaryPredicate>
        bool called_once_with(
            Args... args, const BinaryPredicate& predicate) const
        {
            return m_call_handler.called_once_with(args..., predicate);
        }

        bool called_with(Args... args) const
        {
            return m_call_handler.called_with(args...);
        }

        template<class BinaryPredicate>
        bool called_with(
            Args... args, const BinaryPredicate& predicate) const
        {
            return m_call_handler.called_with(args..., predicate);
        }

        return_handler<R> m_return_handler;
        call_handler<Args...> m_call_handler;
    };
}
