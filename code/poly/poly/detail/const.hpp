// Copyright 2012 Pyry Jahkola.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POLY_DETAIL_CONST_HPP_HWFFT6H
#define POLY_DETAIL_CONST_HPP_HWFFT6H

namespace poly {
namespace detail {

template <typename T> T const & const_(T & t) noexcept { return t; }

} // detail
} // poly

#endif // POLY_DETAIL_CONST_HPP_HWFFT6H
