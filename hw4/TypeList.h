#pragma once

#include <iostream>
#include <string>

//Implementation


struct NullType;

template< typename  ... Args >
struct TypeList;

typedef TypeList<> EmptyList;


template< typename  H, typename ... T >
struct TypeList<H, T...> {
  using Head = H;
  using Tail = TypeList<T...>;
};

template< typename  H,  typename ... T >
struct TypeList<H, TypeList<T...>> {
  using Head = H;
  using Tail = TypeList<T...>;
};

//LENGTH
template <typename ...Ts>
struct Length;

template <
    typename... Ts>
struct Length<TypeList<Ts...>> {
  static constexpr int result = 1 + Length<typename TypeList<Ts...>::Tail>::result;
};

template <>
struct Length<TypeList<>> {
  static constexpr int result = 0;
};

//SIZE
template <typename ...Ts>
struct Size;

template <typename... Ts>
struct Size<TypeList<Ts...>> {
  static constexpr size_t result = sizeof(typename TypeList<Ts...>::Head) + Size<typename TypeList<Ts...>::Tail>::result;
};

template <>
struct Size<TypeList<>> {
  static constexpr size_t result = 0;
};

//GET
template <size_t i, typename ...Ts>
struct Get_impl;

template <size_t i>
struct Get_impl<i, TypeList<>> {
  using type = NullType;
};

template <typename... Ts >
struct Get_impl<0, TypeList<Ts...>> {
  using type = typename TypeList<Ts...>::Head;
};

template <size_t i,
    typename... Ts >
struct Get_impl<i, TypeList<Ts...>> {
  using type = typename Get_impl<i-1, typename TypeList<Ts...>::Tail>::type;
};


//ADD
template <size_t i, typename Nt, typename ...Ts>
struct Add_impl;

template <size_t i,
    typename Nt,
    typename... Ts >
struct Add_impl<i, Nt, TypeList<Ts...>> {
  using type = TypeList<typename TypeList<Ts...>::Head , typename Add_impl<i-1, Nt, typename TypeList<Ts...>::Tail >::type>;
};

template <typename Nt,
    typename... Ts >
struct Add_impl<0, Nt, TypeList<Ts...>> {
  using type = TypeList<Nt, TypeList<Ts...>>;
};

template <typename Nt>
struct Add_impl<0, Nt, TypeList<>> {
  using type = TypeList<Nt>;
};

//DELETE
template <size_t i, typename ...Ts>
struct Delete_impl;

template <size_t i,
    typename... Ts >
struct Delete_impl<i, TypeList<Ts...>> {
  using type = TypeList<typename TypeList<Ts...>::Head , typename Delete_impl<i-1, typename TypeList<Ts...>::Tail>::type>;
};

template <typename... Ts >
struct Delete_impl<0, TypeList<Ts...>> {
  using type = typename TypeList<Ts...>::Tail ;
};

template <size_t i>
struct Delete_impl<i, TypeList<>> {
  using type = NullType;
};

//End of Implementation


template <size_t i, typename ...Ts>
using Get = typename Get_impl<i, Ts...>::type;

template <size_t i, typename Nt, typename ...Ts>
using Add = typename Add_impl<i, Nt, Ts...>::type;

template <size_t i, typename ...Ts>
using Delete = typename Delete_impl<i, Ts...>::type;

template <size_t i, typename T, typename ...Ts>
using Replace = Add<i, T, Delete<i, Ts...>>;
