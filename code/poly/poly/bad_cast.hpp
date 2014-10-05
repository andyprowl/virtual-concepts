// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_BAD_INTERFACE_CAST_HPP_K4SVF0Q
#define POLY_BAD_INTERFACE_CAST_HPP_K4SVF0Q

/// Header <poly/bad_cast.hpp>
/// ====================================
///
/// The `poly::bad_cast` exception.
///
///
/// Class `poly::bad_cast`
/// --------------------------------
///
/// Thrown when `poly::cast<T>(x)` fails for a non-pointer `x`. Inherits
/// `std::bad_cast`.

#include <typeinfo>

namespace poly {

struct bad_cast : std::bad_cast {
    virtual char const * what() const noexcept override {
        return "bad_cast";
    }
};

} // poly

#endif // POLY_BAD_INTERFACE_CAST_HPP_K4SVF0Q
