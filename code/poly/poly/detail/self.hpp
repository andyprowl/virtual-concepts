// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_SELF_HPP_1PQ4JF0
#define POLY_DETAIL_SELF_HPP_1PQ4JF0

#include <poly/detail/strip.hpp>

namespace poly {

struct self {};

// -----------------------------------------------------------------------------

namespace detail {

using poly::self;

// --- self_to_this(a, t) ------------------------------------------------------

template <typename Arg, typename This>
Arg && self_to_this(Arg && a, This &&) noexcept { return std::forward<Arg>(a); }

template <typename This>
This && self_to_this(self, This && t) noexcept { return std::forward<This>(t); }

// --- self_to_this_<Arg, This> ------------------------------------------------

template <typename Arg, typename This>
struct self_to_this_                  { typedef Arg type; };
template <typename T>
struct self_to_this_<self, T>         { typedef T type; };
template <typename T>
struct self_to_this_<self &&, T>      { typedef T && type; };
template <typename T>
struct self_to_this_<self &, T>       { typedef T & type; };
template <typename T>
struct self_to_this_<self const &, T> { typedef T const & type; };

// --- forward_self<Arg>()(arg) ------------------------------------------------

template <typename Arg> struct forward_self {
    template <typename T>
    Arg && operator()(T && a) const noexcept { return std::forward<Arg>(a); }
};
template <> struct forward_self<self> {
    template <typename T>
    self operator()(T &&) const noexcept { return self(); }
};
template <> struct forward_self<self &&> : forward_self<self> {};
template <> struct forward_self<self &> {
    self s;
    template <typename T>
    self & operator()(T &) noexcept { return s; }
};
template <> struct forward_self<self const &> {
    self s;
    template <typename T>
    self const & operator()(T const &) const noexcept { return s; }
};

// --- self_from<Args...> ------------------------------------------------------

template <typename... Args> struct self_from;
template <typename Arg, typename... More>
struct self_from<Arg, More...> : self_from<More...> {
    template <typename First, typename... Rest>
    static auto apply(First &&, Rest &&... rest)
    POLY_RETURNS(self_from<More...>::apply(std::forward<Rest>(rest)...));
};
template <typename... More> struct self_from<self, More...> {
    typedef self type;
    template <typename First, typename... Rest>
    static auto apply(First && first, Rest &&...)
    POLY_RETURNS(std::forward<First>(first));
};
template <typename... More> struct self_from<self &&, More...> {
    typedef self && type;
    template <typename F, typename... Rest>
    static typename detail::strip<F>::type &&
    apply(F && first, Rest &&...) noexcept {
        return std::move(first);
    }
};
template <typename... More> struct self_from<self &, More...> { 
    typedef self & type;
    template <typename First, typename... Rest>
    static First & apply(First & first, Rest &&...) noexcept { return first; }
};
template <typename... More> struct self_from<self const &, More...> {
    typedef self const & type;
    template <typename F, typename... Rs>
    static F const & apply(F const & first, Rs &&...) noexcept { return first; }
};
template <> struct self_from<> {
    template <typename... Rest>
    static void apply(Rest &&...) {
        static_assert(sizeof...(Rest) >= 0, "missing poly::self in signature!");
    }
};

// --- self_from_signature<R(F, Args0..., self [[const] &], Args1...)> ---------

template <typename Sig> struct self_from_signature { typedef void type; };
template <typename R, typename F, typename... Args>
struct self_from_signature<R(F, Args...)> : self_from<Args...> {};

} // detail
} // poly

#endif // POLY_DETAIL_SELF_HPP_1PQ4JF0
