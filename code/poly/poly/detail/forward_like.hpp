// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_FORWARD_LIKE_HPP_7AF5HLG
#define POLY_DETAIL_FORWARD_LIKE_HPP_7AF5HLG

namespace poly {
namespace detail {

// --- fwd<T, U>::type ---------------------------------------------------------

template <typename T, typename U> struct fwd { typedef U type; };
template <typename T, typename U> struct fwd<T, U &>       : fwd<T, U> {};
template <typename T, typename U> struct fwd<T, U const>   : fwd<T, U> {};
template <typename T, typename U> struct fwd<T, U const &> : fwd<T, U> {};
template <typename T, typename U> struct fwd<T &, U> { typedef U & type; };
template <typename T, typename U> struct fwd<T &, U &> { typedef U & type; };
template <typename T, typename U> struct fwd<T const &, U> {
    typedef U const & type;
};
template <typename T, typename U> struct fwd<T const &, U &> {
    typedef U const & type;
};
template <typename T, typename U> struct fwd<T const &, U const &> {
    typedef U const & type;
};

// --- forward_like<T>(u) ------------------------------------------------------

template <typename T, typename U>
typename fwd<T, U>::type && forward_like(U && u) noexcept {
    return static_cast<typename fwd<T, U>::type &&>(u);
}

} // detail
} // poly

#endif // POLY_DETAIL_FORWARD_LIKE_HPP_7AF5HLG
