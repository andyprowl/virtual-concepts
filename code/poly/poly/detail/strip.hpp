// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_STRIP_HPP_LLIN843
#define POLY_DETAIL_STRIP_HPP_LLIN843

namespace poly {
namespace detail {

template <typename T> struct strip { typedef T type; };
template <typename T> struct strip<T &>     : strip<T> {};
template <typename T> struct strip<T &&>    : strip<T> {};
template <typename T> struct strip<T const> : strip<T> {};

} // detail
} // poly

#endif // POLY_DETAIL_STRIP_HPP_LLIN843
