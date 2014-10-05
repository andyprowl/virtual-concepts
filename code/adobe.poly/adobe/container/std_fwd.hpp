/*
    Copyright 2008 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef ADOBE_STD_FWD_HPP
#define ADOBE_STD_FWD_HPP

#include <adobe/config.hpp>

/*************************************************************************************************/

#if BOOST_WORKAROUND(__GNUC__, >= 3) && defined(_GLIBCXX_DEBUG)

#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>

#else

namespace std {

template <typename, typename> class vector;
template <typename, typename> class deque;
template <typename, typename> class list;
template <typename, typename, typename> class set;
template <typename, typename, typename> class multiset;
template <typename, typename, typename, typename> class map;
template <typename, typename, typename, typename> class multimap;

} // namespace std

#endif

/*************************************************************************************************/

#endif

/*************************************************************************************************/
