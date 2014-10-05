// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_SIGNATURES_HPP_NNDM8B4
#define POLY_DETAIL_SIGNATURES_HPP_NNDM8B4

#include <poly/self.hpp>
#include <poly/detail/ref_macros.hpp>
#include <poly/detail/seq.hpp>

namespace poly {
namespace detail {

template <typename Seq,
          typename Self =
          typename self_from_signature<typename head<Seq>::type>::type>
struct signatures {
    void operator()() const noexcept {}
};

template <typename R, typename F, typename... A, typename... Sig>
struct signatures<seq<R(F, A...), Sig...>, self> : signatures<seq<Sig...>> {
    using signatures<seq<Sig...>>::operator();
    virtual R operator()(F, A...) POLY_DETAIL_RREF = 0;
};

template <typename R, typename F, typename... A, typename... Sig>
struct signatures<seq<R(F, A...), Sig...>, self &&> : signatures<seq<Sig...>> {
    using signatures<seq<Sig...>>::operator();
    virtual R operator()(F, A...) POLY_DETAIL_RREF = 0;
};

template <typename R, typename F, typename... A, typename... Sig>
struct signatures<seq<R(F, A...), Sig...>, self &> : signatures<seq<Sig...>> {
    using signatures<seq<Sig...>>::operator();
    virtual R operator()(F, A...) POLY_DETAIL_LREF = 0;
};

template <typename R, typename F, typename... A, typename... Sig>
struct signatures<seq<R(F, A...), Sig...>, self const &>
    : signatures<seq<Sig...>>
{
    using signatures<seq<Sig...>>::operator();
    virtual R operator()(F, A...) const POLY_DETAIL_LREF = 0;
};

} // detail
} // poly

#endif // POLY_DETAIL_SIGNATURES_HPP_NNDM8B4
