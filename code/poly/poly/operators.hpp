// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_OPERATORS_HPP_H9KLZG1
#define POLY_OPERATORS_HPP_H9KLZG1

/// Header <poly/operators.hpp>
/// ===========================
///
///     operator_eq
///     operator_ne
///     operator_lt
///     operator_le
///     operator_gt
///     operator_ge
///     operator_or
///     operator_and
///     operator_not
///     operator_compl
///     operator_add
///     operator_sub
///     operator_mul
///     operator_div
///     operator_mod
///     operator_xor
///     operator_bitor
///     operator_bitand
///     operator_lshift
///     operator_rshift
///     operator_iadd
///     operator_isub
///     operator_imul
///     operator_idiv
///     operator_imod
///     operator_ixor
///     operator_ibitor
///     operator_ibitand
///     operator_ilshift
///     operator_irshift

#include <poly/callable.hpp>

#define POLY_UNARY_OPERATOR_CALL(name, op) /**/ \
    template <typename A>                       \
    constexpr auto call(name##_, A && a)        \
    POLY_RETURNS(op std::forward<A>(a))         \
    /**/

#define POLY_BINARY_OPERATOR_CALL(name, op) /*************/ \
    template <typename A, typename B>                       \
    constexpr auto call(name##_, A && a, B && b)            \
    POLY_RETURNS(std::forward<A>(a) op std::forward<B>(b))  \
    /**/

#define POLY_UNARY_OPERATOR(name, op) /***/ \
    POLY_CALLABLE(name);                    \
    POLY_UNARY_OPERATOR_CALL(name, op)      \
    /**/

#define POLY_BINARY_OPERATOR(name, op) /**/ \
    POLY_CALLABLE(name);                    \
    POLY_BINARY_OPERATOR_CALL(name, op)     \
    /**/

namespace poly {

POLY_BINARY_OPERATOR(operator_eq, ==);          // (a, b)  ~>  a == b
POLY_BINARY_OPERATOR(operator_ne, !=);          // (a, b)  ~>  a != b
POLY_BINARY_OPERATOR(operator_lt, <);           // (a, b)  ~>  a < b
POLY_BINARY_OPERATOR(operator_le, <=);          // (a, b)  ~>  a <= b
POLY_BINARY_OPERATOR(operator_gt, >);           // (a, b)  ~>  a > b
POLY_BINARY_OPERATOR(operator_ge, >=);          // (a, b)  ~>  a >= b

POLY_BINARY_OPERATOR(operator_or,     ||);      // (a, b)  ~>  a || b
POLY_BINARY_OPERATOR(operator_and,    &&);      // (a, b)  ~>  a && b
POLY_UNARY_OPERATOR(operator_not,   !);         // (a)     ~>  !a
POLY_UNARY_OPERATOR(operator_compl, ~);         // (a)     ~>  ~a

POLY_CALLABLE(operator_add);
POLY_UNARY_OPERATOR_CALL(operator_add, +);      // (a)     ~>  +a
POLY_BINARY_OPERATOR_CALL(operator_add, +);     // (a, b)  ~>  a + b

POLY_CALLABLE(operator_sub);
POLY_UNARY_OPERATOR_CALL(operator_sub, -);      // (a)     ~>  -a
POLY_BINARY_OPERATOR_CALL(operator_sub, -);     // (a, b)  ~>  a - b

POLY_BINARY_OPERATOR(operator_mul, *);          // (a, b)  ~>  a * b
POLY_BINARY_OPERATOR(operator_div, /);          // (a, b)  ~>  a / b
POLY_BINARY_OPERATOR(operator_mod, %);          // (a, b)  ~>  a % b

POLY_BINARY_OPERATOR(operator_xor,    ^);       // (a, b)  ~>  a ^ b
POLY_BINARY_OPERATOR(operator_bitor,  |);       // (a, b)  ~>  a | b
POLY_BINARY_OPERATOR(operator_bitand, &);       // (a, b)  ~>  a & b
POLY_BINARY_OPERATOR(operator_lshift, <<);      // (a, b)  ~>  a << b
POLY_BINARY_OPERATOR(operator_rshift, >>);      // (a, b)  ~>  a >> b

POLY_BINARY_OPERATOR(operator_iadd,    +=);     // (a, b)  ~>  a += b
POLY_BINARY_OPERATOR(operator_isub,    -=);     // (a, b)  ~>  a -= b
POLY_BINARY_OPERATOR(operator_imul,    *=);     // (a, b)  ~>  a *= b
POLY_BINARY_OPERATOR(operator_idiv,    /=);     // (a, b)  ~>  a /= b
POLY_BINARY_OPERATOR(operator_imod,    %=);     // (a, b)  ~>  a %= b
POLY_BINARY_OPERATOR(operator_ixor,    ^=);     // (a, b)  ~>  a ^= b
POLY_BINARY_OPERATOR(operator_ibitor,  |=);     // (a, b)  ~>  a |= b
POLY_BINARY_OPERATOR(operator_ibitand, &=);     // (a, b)  ~>  a &= b
POLY_BINARY_OPERATOR(operator_ilshift, <<=);    // (a, b)  ~>  a <<= b
POLY_BINARY_OPERATOR(operator_irshift, >>=);    // (a, b)  ~>  a >>= b

POLY_UNARY_OPERATOR(operator_incr, ++);         // (a)     ~>  ++a
template <typename A, typename B>               // (a, 0)  ~>  a++
constexpr auto call(operator_incr_, A && a, B && b)
POLY_RETURNS(std::forward<A>(a).operator++(std::forward<B>(b)));

POLY_UNARY_OPERATOR(operator_decr, --);         // (a)     ~>  --a
template <typename A, typename B>               // (a, 0)  ~>  a--
constexpr auto call(operator_decr_, A && a, B && b)
POLY_RETURNS(std::forward<A>(a).operator--(std::forward<B>(b)));

POLY_UNARY_OPERATOR(operator_addressof, &);     // (a)     ~>  &a
POLY_UNARY_OPERATOR(operator_indirect,  *);     // (a)     ~>  *a
POLY_BINARY_OPERATOR(operator_member,   ->*);   // (a, b)  ~>  a ->* b

POLY_CALLABLE(operator_call);                   // (a, b...)  ~>  a(b...)
template <typename F, typename... Args>
constexpr auto call(operator_call_, F && f, Args &&... args)
POLY_RETURNS(std::forward<F>(f)(std::forward<Args>(args)...));

POLY_CALLABLE(operator_subscript);              // (a, b)  ~>  a[b]
template <typename A, typename B>
constexpr auto call(operator_subscript_, A && a, B && b)
POLY_RETURNS(std::forward<A>(a)[std::forward<B>(b)]);

POLY_CALLABLE(operator_comma);                  // (a, b)  ~>  a , b
template <typename A, typename B>
constexpr auto call(operator_comma_, A && a, B && b)
POLY_RETURNS(std::forward<A>(a) , std::forward<B>(b));

} // poly

#endif // POLY_OPERATORS_HPP_H9KLZG1
