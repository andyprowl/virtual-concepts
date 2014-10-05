// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_IS_PLAIN_HPP_C59CAS1
#define POLY_DETAIL_IS_PLAIN_HPP_C59CAS1

#include <type_traits>

namespace poly {
namespace detail {

template <typename T> struct is_plain            : std::true_type {};
template <typename T> struct is_plain<T &>       : std::false_type {};
template <typename T> struct is_plain<T const>   : std::false_type {};
template <typename T> struct is_plain<T const &> : std::false_type {};

} // detail
} // poly

#endif // POLY_DETAIL_IS_PLAIN_HPP_C59CAS1
