// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_INTERFACE_HPP_CHA52S4
#define POLY_INTERFACE_HPP_CHA52S4

/// Header <poly/interface.hpp>
/// ===========================
///
/// Define a polymorphic value type, given a set of callable signatures, which
/// accepts any value for which those callables are properly implemented.
///
/// **Remark.** Unlike with class-based polymorphism, i.e. pure virtual classes,
/// the callables can be implemented without modifying the definition of the
/// interface or the type to be instantiated.
///
///
/// Class template `poly::interface<Signatures...>`
/// -----------------------------------------------
///
///
///
/// Class template `poly::interface<Interface, Signatures...>`
/// ----------------------------------------------------------
///
/// **Example.**
/// 
/// #include <poly/interface.hpp>
/// #include <iostream>
/// 
/// namespace ns {
/// 
///     POLY_CALLABLE(increment); // increment(x), increment(in_place, x)
/// 
///     constexpr struct in_place_ {} in_place = {};
/// 
///     template <typename T> T call(increment_, T t) {
///         ns::increment(in_place, t);
///         return t;
///     }
/// 
///     struct incrementable : poly::interface<incrementable
///       , void(increment_, in_place_, poly::self &)
///     > { POLY_INTERFACE_CONSTRUCTORS(incrementable); };
/// 
///     void call(increment_, in_place_, int & i) { i++; }
/// 
/// } // ns
/// 
/// int main() {
///     ns::incrementable x = 0;
///     auto y = ns::increment(x);
///     std::cout << "result: " << poly::cast<int>(y) << std::endl;
/// }

// -----------------------------------------------------------------------------

#include <poly/bad_cast.hpp>
#include <poly/callable.hpp>
#include <poly/detail/friends.hpp>
#include <poly/detail/is_plain.hpp>
#include <poly/detail/implement.hpp>
#include <poly/detail/signatures.hpp>
#include <poly/detail/strip.hpp>
#include <poly/detail/config.hpp>
#include <type_traits>
#include <memory>
#include <cassert>

#define POLY_INTERFACE_CONSTRUCTORS(cls) /*****************/ \
    template <typename... A>                                 \
    cls(A &&... args)                                        \
    noexcept(noexcept(base(std::forward<A>(args)...)))       \
    : base(std::forward<A>(args)...) {}                      \
    typedef int POLY_DETAIL_CAT(POLY_CONSTRUCTOR_, __LINE__) \
    /**/


namespace poly {


template <typename... Signatures> struct interface;


// template <>
// struct interface<> : interface<interface<>>
// {
//     typedef interface<interface<>> base;
//     POLY_INTERFACE_CONSTRUCTORS(interface);
// };


template <typename R, typename F, typename... Args, typename... Sigs>
struct interface<R(F, Args...), Sigs...>
    : interface<interface<R(F, Args...), Sigs...>, R(F, Args...), Sigs...>
{
    typedef interface<interface<R(F, Args...), Sigs...>,
                                R(F, Args...), Sigs...> base;
    POLY_INTERFACE_CONSTRUCTORS(interface);
};


template <typename Interface, typename... Signatures>
struct interface<Interface, Signatures...>
    : detail::friends<Interface, detail::signature<Signatures>...>
{
    typedef interface base;

    struct concept
#ifndef POLY_NO_MULTIPLE_INHERITANCE
        : detail::signature<Signatures>...
#else
        : detail::signatures<detail::seq<Signatures...>>
#endif
    {
        virtual ~concept() = default;
        virtual concept * copy() const = 0;
        virtual void * data() noexcept = 0;
        virtual void const * data() const noexcept = 0;
        virtual std::type_info const & type() const noexcept = 0;
    };

    template <typename T>
    struct model
        : detail::implement<model<T>, concept, detail::signature<Signatures>...>
    {
        static_assert(detail::is_plain<T>::value, "unusable type!");
        model(T && x) : x(std::move(x)) {}
        template <typename... Args>
        explicit model(Args &&... args) : x(std::forward<Args>(args)...) {}
        virtual concept * copy() const override { return new model(*this); }
        virtual void * data() noexcept override { return &x; }
        virtual void const * data() const noexcept override { return &x; }
        virtual std::type_info const & type() const noexcept override {
            return typeid(T);
        }
        T x;
    };

    template <typename T, typename... Args>
    static interface make(Args &&... args) {
        return interface(new model<T>(std::forward<Args>(args)...));
    }

    interface() noexcept = default;
    interface(interface &&) noexcept = default;
    interface(interface const & x) : p(x.p->copy()) {}
    interface(Interface const & x) : p(x.p->copy()) {}
    template <typename T>
    interface(T x) : p(new model<T>(std::move(x))) {}

    interface & operator=(interface x) noexcept {
        p = std::move(x.p);
        return *this;
    }

    bool valid() const noexcept { return static_cast<bool>(p); }

    concept & get() POLY_DETAIL_LREF noexcept {
        assert(valid());
        return *p;
    }
    concept const & get() const POLY_DETAIL_LREF noexcept {
        assert(valid());
        return *p;
    }
#ifndef POLY_NO_REF_QUALIFIERS
    concept && get() && noexcept {
        assert(valid());
        return std::move(*p);
    }
#endif

    std::type_info const & type() const noexcept {
        assert(valid());
        return p->type();
    }
    void * data() noexcept { return get().data(); }
    void const * data() const noexcept { return get().data(); }

    template <typename T> T & get() POLY_DETAIL_LREF {
        if (type() != typeid(T)) throw bad_cast();
        return *static_cast<T *>(data());
    }
    template <typename T> T const & get() const POLY_DETAIL_LREF {
        if (type() != typeid(T)) throw bad_cast();
        return *static_cast<T const *>(data());
    }
#ifndef POLY_NO_REF_QUALIFIERS
    template <typename T> T && get() && {
        assert(valid());
        static_assert(detail::is_plain<T>::value, "");
        return std::move(*static_cast<T *>(data()));
    }
#endif
    template <typename T> T && move() {
        return std::move((*this).template get<T>());
    }

private:
    explicit interface(concept * p) : p(p) {}
    std::unique_ptr<concept> p;
};


// -----------------------------------------------------------------------------


template <typename T, typename... Sigs>
inline T * cast(interface<Sigs...> * p) noexcept {
    assert(p);
    if (p->type() != typeid(T)) return nullptr;
    return static_cast<T *>(p->data());
}

template <typename T, typename... Sigs>
inline T const * cast(interface<Sigs...> const * p) noexcept {
    assert(p);
    if (p->type() != typeid(T)) return nullptr;
    return static_cast<T const *>(p->data());
}

template <typename T, typename... Sigs>
inline T && cast(interface<Sigs...> && x) {
    return x.template move<T>();
}

template <typename T, typename... Sigs>
inline T & cast(interface<Sigs...> & x) {
    return x.template get<T>();
}

template <typename T, typename... Sigs>
inline T const & cast(interface<Sigs...> const & x) {
    return x.template get<T>();
}


} // poly

#endif // POLY_INTERFACE_HPP_CHA52S4
