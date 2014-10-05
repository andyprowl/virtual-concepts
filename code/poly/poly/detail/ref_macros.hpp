// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_REF_MACROS_HPP_C045URO
#define POLY_DETAIL_REF_MACROS_HPP_C045URO

#include <poly/detail/config.hpp>

#ifndef POLY_NO_REF_QUALIFIERS
#define POLY_DETAIL_LREF &
#define POLY_DETAIL_RREF &&
#else
#define POLY_DETAIL_LREF
#define POLY_DETAIL_RREF
#endif

#endif // POLY_DETAIL_REF_MACROS_HPP_C045URO
