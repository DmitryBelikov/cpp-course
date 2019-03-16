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

constexpr placeholder<1> _1;
constexpr placeholder<2> _2;
constexpr placeholder<3> _3;

template <bool Once, typename F, typename ... As>
struct bind_t;

template<typename T, typename... Ts>
struct get_count;

template<typename T>
struct get_count<T> {
    constexpr static int value = 0;
};

template<typename T0, typename T1, typename... Ts>
struct get_count<T0, T1, Ts...> {
    constexpr static int value = static_cast<int>(std::is_same_v<T0, T1>) + get_count<T0, Ts...>::value;
};

template<typename T0, bool Once, typename F, typename... Args, typename... Ts>
struct get_count<T0, bind_t<Once, F, Args...>, Ts...> {
    constexpr static int value = get_count<T0, Args...>::value + get_count<T0, Ts...>::value;
};

template<typename T0, typename... Ts>
struct is_unique {
    constexpr static bool value = get_count<T0, Ts...>::value == 1;
};

template<bool Once, typename T>
struct add_ref;

template<typename T>
struct add_ref<false, T> {
    typedef T& type;
};

template<typename T>
struct add_ref<true, T> {
    typedef T&& type;
};

template<typename B, int N, typename... Bs>
struct add_arg_ref {
    typedef typename add_ref<get_count<const placeholder<N + 1>&, Bs...>::value == 1, B>::type type;
};

template<typename B, int N, typename... Bs>
using add_arg_ref_t = typename add_arg_ref<B, N, Bs...>::type;

template<bool Once, typename A>
struct G
{
    G(A&& a)
        : a(std::forward<A>(a))
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ...)
    {
        return static_cast<typename add_ref<Once, std::decay_t<A>>::type>(a);
    }

    std::decay_t<A> a;
};

//template <typename A>
//struct G<false, A>
//{
//    G(A&& a)
//        : a(std::forward<A>(a))
//    {}

//    template <typename ... Bs>
//    decltype(auto) operator()(Bs&& ...)
//    {
//        return static_cast<std::decay_t<A>&>(a);
//    }

//    std::decay_t<A> a;
//};

//template <typename A>
//struct G<true, A>
//{
//    G(A&& a)
//        : a(std::forward<A>(a))
//    {}

//    template <typename ... Bs>
//    decltype(auto) operator()(Bs&& ...)
//    {
//        return a;
//    }

//    std::decay_t<A> a;
//};


template <bool Once>
struct G<Once, const placeholder<1>&>
{
    G(const placeholder<1>&)
    {}

    template <typename B1, typename ... Bs>
    decltype(auto) operator()(B1&& b1, Bs&& ...)
    {
        return std::forward<B1>(b1);
    }
};

template <bool Once, int N>
struct G<Once, const placeholder<N>& >
{
    G(const placeholder<N>&)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator()(B&&, Bs&& ... bs)
    {
        G<Once, const placeholder<N - 1>&> next((placeholder<N - 1>()));
        return next(std::forward<Bs>(bs)...);
    }
};

template<bool Once, bool InnerOnce, typename F, typename ... As>
struct G<Once, bind_t<InnerOnce, F, As...>> {
    G(const bind_t<InnerOnce, F, As...>& fun)
        : fun(fun)
    {}

    G(bind_t<InnerOnce, F, As...>&& fun)
        : fun(std::forward<bind_t<InnerOnce, F, As...>>(fun))
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ... bs)
    {
        return fun(std::forward<Bs>(bs)...);
    }

    bind_t<InnerOnce, F, As...> fun;
};

template <bool Once, typename F, typename ... As>
struct bind_t
{
    bind_t(F&& f, As&& ... as)
        : f(std::forward<F>(f))
        , gs(std::forward<As>(as)...)
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ... bs)
    {
        return call(typename make_integer_sequence<int, sizeof...(As)>::type(),
                    typename make_integer_sequence<int, sizeof...(Bs)>::type(), std::forward<Bs>(bs)...);
    }

private:
    template <int... ks, int... ns, typename ... Bs>
    decltype(auto) call(integer_sequence<int, ks...>, integer_sequence<int, ns...>, Bs&& ... bs)
    {

        return f(std::get<ks>(gs)(add_arg_ref_t<Bs, ns, As...>(bs)...)...);
    }

private:
    std::decay_t<F> f;
    std::tuple<G<Once, As>...> gs;
};

template <typename F, typename ... As>
bind_t<false, F, As...> bind(F&& f, As&& ... as)
{
    return bind_t<false, F, As...>(std::forward<F>(f), std::forward<As>(as)...);
}

template<typename F, typename... As>
bind_t<true, F, As...> call_once_bind(F&& f, As&& ... as)
{
    return bind_t<true, F, As...>(std::forward<F>(f), std::forward<As>(as)...);
}

#endif	// BIND_H
