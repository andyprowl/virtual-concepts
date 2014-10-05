// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_FRIENDS_HPP_UIZR5HW
#define POLY_DETAIL_FRIENDS_HPP_UIZR5HW

#include <poly/detail/forward_like.hpp>
#include <poly/detail/signature.hpp>

namespace poly {
namespace detail {

template <typename Interface, typename... Signatures> struct friends;

template <typename Interface> struct friends<Interface> {};

template <typename I,
          typename R, typename F, typename... Args, typename Self,
          typename... Signatures>
struct friends<I, signature<R(F, Args...), Self>, Signatures...>
    : friends<I, Signatures...>
{
    friend R call(F f, typename self_to_this_<Args, I>::type... args) {
        return forward_like<Self>(self_from<Args...>::apply(
            std::forward<typename self_to_this_<Args, I>::type>(args)...).get())
            (f, forward_self<Args>()(args)...);
    }
};

template <typename I, typename F, typename... Args, typename Self,
          typename... Signatures>
struct friends<I, signature<void(F, Args...), Self>, Signatures...>
    : friends<I, Signatures...>
{
    friend void call(F f, typename self_to_this_<Args, I>::type... args) {
        forward_like<Self>(self_from<Args...>::apply(
            std::forward<typename self_to_this_<Args, I>::type>(args)...).get())
            (f, forward_self<Args>()(args)...);
    }
};

} // detail
} // poly

#endif // POLY_DETAIL_FRIENDS_HPP_UIZR5HW
