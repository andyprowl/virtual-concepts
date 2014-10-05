// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_RETURNS_HPP_3LKOAC9
#define POLY_RETURNS_HPP_3LKOAC9

/// Header <poly/returns.hpp>
/// =========================
///
/// Simplify the return type, noexcept, and return value computation.
///
///
/// Macro `POLY_RETURNS(expression)`
/// --------------------------------
///
/// Avoid duplication in computation of return type, the noexcept specifier, and
/// return value in a one-expression function definition. To be used with `auto`
/// like the following example illustrates:
///
///     template <typename A, typename B>
///     auto plus(A && a, B && b) POLY_RETURNS(a + b);
///
/// **Remark.** Append semicolon where this macro is used.
/// 
/// **Remark.** If `POLY_NO_RETURNS_NOEXCEPT` is defined, this macro reduces to
/// `POLY_RETURNS_THROW(expression)`.
///
/// **See also.** `POLY_RETURNS_THROW(expression)`
///
///
/// Macro `POLY_RETURNS_THROW(expression)`
/// --------------------------------------
///
/// Avoid duplication in computation of return type and return value in a
/// one-expression function definition. To be used with `auto` like the
/// following example illustrates:
///
///     template <typename A, typename B>
///     auto plus(A && a, B && b) POLY_RETURNS_THROW(a + b);
///
/// **Remark.** Append semicolon where this macro is used.
/// 
/// **See also**: `POLY_RETURNS(expression)`

#include <poly/detail/cat.hpp>

// -----------------------------------------------------------------------------

#define POLY_RETURNS_THROW(...) /**********************/ \
    -> decltype(__VA_ARGS__)  { return (__VA_ARGS__); }  \
    typedef int POLY_DETAIL_CAT(POLY_RETURNS_, __LINE__) \
    /**/

// -----------------------------------------------------------------------------

#ifndef POLY_NO_RETURNS_NOEXCEPT
#   define POLY_RETURNS(...) /************************************/ \
        noexcept(noexcept(decltype(__VA_ARGS__)                     \
            (std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__))))    \
        -> decltype(__VA_ARGS__)  { return (__VA_ARGS__); }         \
        typedef int POLY_DETAIL_CAT(POLY_RETURNS_, __LINE__)        \
        /**/
#else
#   define POLY_RETURNS(...) POLY_RETURNS_THROW(__VA_ARGS__)
#endif

#endif // POLY_RETURNS_HPP_3LKOAC9
