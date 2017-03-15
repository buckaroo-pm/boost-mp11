
//  Copyright 2015 Peter Dimov.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt


#include <boost/mp11/utility.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>

using boost::mp11::mp_unquote;

template<class...> struct X {};

template<template<class...> class F, class... T> using Y = X<F<T>...>;

template<class Q, class... T> using Z = X<mp_unquote<Q, T>...>;

struct B {};
struct D1: B {};
struct D2: B {};
struct ND {};

template<class... T> using is_base_of_t = typename std::is_base_of<T...>::type;

int main()
{
    using boost::mp11::mp_identity_t;
    using boost::mp11::mp_quote;

    {
        using Q = mp_quote<mp_identity_t>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_unquote<Q, void>, void>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_unquote<Q, int[]>, int[]>));
    }

    {
        using Q = mp_quote<std::is_same, void>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_unquote<Q, void>, std::is_same<void, void>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_unquote<Q, int[]>, std::is_same<void, int[]>>));
    }

    {
        using Q = mp_quote<X, char[1], char[2], char[3]>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_unquote<Q, int[1], int[2], int[3]>, X<char[1], char[2], char[3], int[1], int[2], int[3]>>));
    }

    {
        using Q = mp_quote<mp_identity_t>;

        // using R1 = Y<Q::template apply, void, char, int>;
        // BOOST_TEST_TRAIT_TRUE((std::is_same<R1, X<void, char, int>>));
        // 
        // error: pack expansion used as argument for non-pack parameter of alias template

#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, <= 1910 && BOOST_MSVC >= 1900 )
#else
        using R2 = Z<Q, void, char, int>;
        BOOST_TEST_TRAIT_TRUE((std::is_same<R2, X<void, char, int>>));
#endif
    }

    {
        using Q = mp_quote<is_base_of_t, B>;

#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, <= 1800 )
#else
        using R1 = Y<Q::template apply, D1, D2, ND, int>;
        BOOST_TEST_TRAIT_TRUE((std::is_same<R1, X<std::true_type, std::true_type, std::false_type, std::false_type>>));
#endif

#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, <= 1910 && BOOST_MSVC >= 1900 )
#else
        using R2 = Z<Q, D1, D2, ND, int>;
        BOOST_TEST_TRAIT_TRUE((std::is_same<R2, X<std::true_type, std::true_type, std::false_type, std::false_type>>));
#endif
    }

    return boost::report_errors();
}
