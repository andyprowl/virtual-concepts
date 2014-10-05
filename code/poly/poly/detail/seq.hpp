// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_SEQ_HPP_XLJV79Q
#define POLY_DETAIL_SEQ_HPP_XLJV79Q

namespace poly {
namespace detail {

// --- seq<T...>, head<Seq>::type ----------------------------------------------

template <typename... T> struct seq {};

template <typename Seq> struct head { typedef void type; };
template <typename H, typename... T>
struct head<seq<H, T...>> { typedef H type; };

} // detail
} // poly

#endif // POLY_DETAIL_SEQ_HPP_XLJV79Q
