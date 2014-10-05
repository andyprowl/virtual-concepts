// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_FORWARD_HPP_S2NYKAD
#define POLY_FORWARD_HPP_S2NYKAD

/// Header <poly/forward.hpp>
/// =========================
///
/// `poly::forward<T>(t)` with `constexpr` support.
///
///
/// Function template `poly::forward<T>(t)`
/// ---------------------------------------
///
/// Reimplementation of `std::forward<T>(t)` from the C++11 standard, with
/// added `constexpr` support.

#include <type_traits>

namespace poly {
    
template <typename T>
inline constexpr T &&
forward(typename std::remove_reference<T>::type & t) noexcept {
    return static_cast<T &&>(t);
}
template <typename T>
inline constexpr T &&
forward(typename std::remove_reference<T>::type && t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "");
    return static_cast<T &&>(t);
}

} // poly

#endif // POLY_FORWARD_HPP_S2NYKAD
