// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_SIGNATURE_HPP_NUZPJW3
#define POLY_DETAIL_SIGNATURE_HPP_NUZPJW3

#include <poly/self.hpp>
#include <poly/detail/ref_macros.hpp>

namespace poly {
namespace detail {

template <typename Sig, typename Self=typename self_from_signature<Sig>::type>
struct signature;

template <typename R, typename F, typename... A>
struct signature<R(F, A...), self> {
    virtual R operator()(F, A...) POLY_DETAIL_RREF = 0;
};

template <typename R, typename F, typename... A>
struct signature<R(F, A...), self &&> {
    virtual R operator()(F, A...) POLY_DETAIL_RREF = 0;
};

template <typename R, typename F, typename... A>
struct signature<R(F, A...), self &> {
    virtual R operator()(F, A...) POLY_DETAIL_LREF = 0;
};

template <typename R, typename F, typename... A>
struct signature<R(F, A...), self const &> {
    virtual R operator()(F, A...) const POLY_DETAIL_LREF = 0;
};

} // detail
} // poly

#endif // POLY_DETAIL_SIGNATURE_HPP_NUZPJW3
