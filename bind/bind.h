#ifndef BIND_H
#define BIND_H
#include <iostream>
#include <tuple>

template <typename T, T ... values>
struct integer_sequence
{};

template <typename T, typename Seq, T arg>
struct append;

template <typename T, T ... values, T arg>
struct append<T, integer_sequence<T, values...>, arg>
{
    typedef integer_sequence<T, values..., arg> type;
};

template <typename T, T N, typename Q = void>
struct make_integer_sequence
{
    typedef typename append<T, typename make_integer_sequence<T, N - 1>::type, N - 1>::type type;
};

template <typename T, T N>
struct make_integer_sequence<T, N, typename std::enable_if<N == 0>::type>
{
    typedef integer_sequence<T> type;
};

template <int N>
struct placeholder
{};

template <typename F, typename ... As>
struct bind_t;

template <typename A>
struct G
{
    G(A&& a)
        : a(std::forward<A>(a))
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ...) const
    {
        return a;
    }

    A a;
};

template <>
struct G<const placeholder<1>& >
{
    G(const placeholder<1>&)
    {}

    template <typename B1, typename ... Bs>
    decltype(auto) operator()(B1&& b1, Bs&& ...) const
    {
        return std::forward<B1>(b1);
    }
};

template <int N>
struct G<const placeholder<N>& >
{
    G(const placeholder<N>&)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator()(B&&, Bs&& ... bs) const
    {
        G<const placeholder<N - 1>&> next((placeholder<N - 1>()));
        return next(std::forward<Bs>(bs)...);
    }
};

template <typename F, typename ... As>
struct G<bind_t<F, As...> >
{
    G(bind_t<F, As...> fun)
        : fun(fun)
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ... bs) const
    {
        return fun(std::forward<Bs>(bs)...);
    }

    bind_t<F, As...> fun;
};

template <typename F, typename ... As>
struct bind_t
{
    bind_t(F&& f, As&& ... as)
        : f(std::forward<F>(f))
        , gs(std::forward<As>(as)...)
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ... bs) const
    {
        return call(typename make_integer_sequence<int, sizeof...(As)>::type(), std::forward<Bs>(bs)...);
    }

private:
    template <int... ks, typename ... Bs>
    decltype(auto) call(integer_sequence<int, ks...>, Bs&& ... bs) const
    {
        return f(std::get<ks>(gs)(std::forward<Bs>(bs)...)...);
    }

private:
    F f;
    std::tuple<G<As>...> gs;
};

template <typename F, typename ... As>
bind_t<F, As...> bind(F&& f, As&& ... as)
{
    return bind_t<F, As...>(std::forward<F>(f), std::forward<As>(as)...);
}

#endif	// BIND_H
