// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_CALLABLE_HPP_3BLYFN2
#define POLY_CALLABLE_HPP_3BLYFN2

/// Header <poly/callable.hpp>
/// ==========================
///
/// Define first-class externally overloadable function objects, compatible with
/// `poly::interface<Sig...>` instances. Declare them by `POLY_CALLABLE(name)`,
/// implement by overloading `result_type call(name_, arguments...)`.
///
///
/// Class template `poly::callable<F>`
/// ----------------------------------
///
/// Define the function call operator for your callable `F` by the means of the
/// overloadable `call(F, ...)` free function.
///
/// This [CRTP][1] base class defines the function call operator for the type
/// `F`. Given an instance `f` of type `F`, the function call operator
/// `f(a, b, c)` expects that the argument-dependent lookup ([ADL][2]) call to
/// `call(f, a, b, c)` is defined. (I recommend you to define `call(f, a, b, c)`
/// always in the namespace of `f`, but in principle, any namespace where ADL
/// kicks in will do.)
///
/// **Usage.** Define a callable type in your selected namespace. Typically,
/// you'll want to create a `constexpr` global instance of it at the same time.
/// The overloads for `call(F, ...)` will probably follow.
///
///     #include <poly/callable.hpp>
///
///     namespace mine {
///         constexpr struct dig_ : poly::callable<dig_> {} dig = {};
///         /// Alternatively: `POLY_CALLABLE(dig);`
///
///         struct rock {};
///         struct gold {};
///
///         gold call(dig_, rock const &) { return gold(); }
///         /// Now you can write `auto au = dig(rock());` to get gold!
///     } // mine
///
/// The convention in this library is to append an underscore to the name of the
/// callable type and use the unsuffixed name for the global instance. For
/// convenience, use the `POLY_CALLABLE(name)` macro to define the class and the
/// instance at once.
///
/// **See also.** `POLY_CALLABLE(name)`, `POLY_CALLABLE_TYPE(type, name)`
///
/// [1]: http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
/// [2]: http://en.wikipedia.org/wiki/Argument-dependent_name_lookup
///
///
/// Macro `POLY_CALLABLE(name)`
/// ---------------------------
///
/// Define a *callable* type named `name_` and a global `constexpr` instance of
/// it named `name` in the current namespace.
///
/// **Remark.** Append semicolon where this macro is used.
///
/// **Example.** The macro invocation
///
///     POLY_CALLABLE(minus);
///
/// expands to
///
///     constexpr struct minus_ : ::poly::callable<minus_> {} minus = {};
///
/// which is then usable (and overloadable) as the type `minus_` as well as the
/// `constexpr` value `minus`:
///
///     template <typename A>
///     auto call(minus_, A && a)
///     POLY_RETURNS(-std::forward<A>(a));
///
///     template <typename A, typename B>
///     auto call(minus_, A && a, B && b)
///     POLY_RETURNS(std::forward<A>(a) - std::forward<B>(b));
///
///     #include <iostream>
///     #include <vector>
///     #include <algorithm>
///
///     int main() {
///         std::cout << minus(4, 1.5) << std::endl;  //=>  2.5
///         std::vector<int> xs = {1, 2, 3};
///         std::vector<int> ys(xs.size());
///         std::transform(begin(xs), end(xs), begin(ys), minus);
///         for (auto const & y : ys) {
///             std::cout << y << ' ';  //=>  -1 -2 -3
///         }
///     }
///
///
/// Macro `POLY_CALLABLE_TYPE(type, name)`
/// --------------------------------------
///
/// Define a *callable* type named `type` and a global `constexpr` instance of
/// it named `name` in the current namespace. Use this macro as an alternative
/// to `POLY_CALLABLE(name)` if the appended underscore in type name is not
/// desired.
///
/// **Remark.** Append semicolon where this macro is used.
///
/// **Example.** The macro invocation
///
///     POLY_CALLABLE_TYPE(Minus, minus);
///
/// expands to
///
///     constexpr struct Minus : ::poly::callable<Minus> {} minus = {}:

// -----------------------------------------------------------------------------

#include <poly/forward.hpp>
#include <poly/returns.hpp>
#include <functional>

#define POLY_CALLABLE_TYPE(type, name) /**********************/ \
    constexpr struct type : ::poly::callable<type> {} name = {} \
    /**/

#define POLY_CALLABLE(name) POLY_CALLABLE_TYPE(name##_, name)

namespace poly {

template <typename F> struct callable {
    template <typename... Args>
    inline constexpr auto operator()(Args &&... args) const
    noexcept(noexcept(decltype(
        call(std::declval<F const &>(), poly::forward<Args>(args)...))(
        call(std::declval<F const &>(), poly::forward<Args>(args)...))))
    -> decltype(
        call(std::declval<F const &>(), poly::forward<Args>(args)...))
    {
        static_assert(std::is_base_of<callable, F>::value,
            "invalid use of CRTP -- F must inherit from callable<F>");
        static_assert(std::is_empty<F>::value,
            "invalid callable -- F must be an empty (stateless) class");
        return call(static_cast<F const &>(*this),
                    poly::forward<Args>(args)...);
    }
};

} // poly

#endif // POLY_CALLABLE_HPP_3BLYFN2
