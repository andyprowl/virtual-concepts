// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_IMPLEMENT_HPP_ZF88244
#define POLY_DETAIL_IMPLEMENT_HPP_ZF88244

#include <poly/detail/signature.hpp>

namespace poly {
namespace detail {

// --- implement<Model, Concept, Signatures...> --------------------------------

template <typename Model, typename Concept, typename... Signatures>
struct implement;

template <typename Model, typename Concept>
struct implement<Model, Concept> : Concept {};

// non-void rvalue

template <typename M, typename C,
          typename R, typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<R(F, Args...), self>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual R operator()(F f, Args... args) POLY_DETAIL_RREF override {
        return call(f, self_to_this(std::forward<Args>(args),
                                    static_cast<M &&>(*this).x)...);
    }
};

template <typename M, typename C,
          typename R, typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<R(F, Args...), self &&>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual R operator()(F f, Args... args) POLY_DETAIL_RREF override {
        return call(f, self_to_this(std::forward<Args>(args),
                                    static_cast<M &&>(*this).x)...);
    }
};

// non-void lvalue

template <typename M, typename C,
          typename R, typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<R(F, Args...), self &>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual R operator()(F f, Args... args) POLY_DETAIL_LREF override {
        return call(f, self_to_this(std::forward<Args>(args),
                                    static_cast<M &>(*this).x)...);
    }
};

template <typename M, typename C,
          typename R, typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<R(F, Args...), self const &>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual R operator()(F f, Args... args) const POLY_DETAIL_LREF override {
        return call(f, self_to_this(std::forward<Args>(args),
                                    static_cast<M const &>(*this).x)...);
    }
};

// void rvalue

template <typename M, typename C,
          typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<void(F, Args...), self>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual void operator()(F f, Args... args) POLY_DETAIL_RREF override {
        call(f, self_to_this(std::forward<Args>(args),
                             static_cast<M &&>(*this).x)...);
    }
};

template <typename M, typename C,
          typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<void(F, Args...), self &&>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual void operator()(F f, Args... args) POLY_DETAIL_RREF override {
        call(f, self_to_this(std::forward<Args>(args),
                             static_cast<M &&>(*this).x)...);
    }
};

// void lvalue

template <typename M, typename C,
          typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<void(F, Args...), self &>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual void operator()(F f, Args... args) POLY_DETAIL_LREF override {
        call(f, self_to_this(std::forward<Args>(args),
                             static_cast<M &>(*this).x)...);
    }
};

template <typename M, typename C,
          typename F, typename... Args,
          typename... Signatures>
struct implement<M, C, signature<void(F, Args...), self const &>, Signatures...>
    : implement<M, C, Signatures...>
{
    virtual void operator()(F f, Args... args) const POLY_DETAIL_LREF override {
        call(f, self_to_this(std::forward<Args>(args),
                             static_cast<M const &>(*this).x)...);
    }
};

} // detail
} // poly

#endif // POLY_DETAIL_IMPLEMENT_HPP_ZF88244
