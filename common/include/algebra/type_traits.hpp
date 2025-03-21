/** Algebra plugins library, part of the ACTS project
 *
 * (c) 2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// System include(s).
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace algebra {

namespace traits {

/// Matrix traits
/// @{

/// Type of the matrix indices
/// @{
template <class M>
struct index {};

template <class M>
using index_t = typename index<M>::type;
/// @}

/// Value type that is used with the matrix (e.g. float or double precision)
/// @{
template <class M>
struct value {};

template <class M>
using value_t = typename value<M>::type;

template <typename T>
requires(std::is_arithmetic_v<T>&& std::is_scalar_v<T>) struct value<T> {
  using type = T;
};
/// @}

/// Scalar type that is used with the matrix (can be multiple values in SoA)
/// @{
template <class M>
struct scalar {
  using type = value_t<M>;
};

template <class M>
using scalar_t = typename scalar<M>::type;
/// @}

/// Vector type that is compatible with the matrix
/// @{
template <class M>
struct vector {};

template <class M>
using vector_t = typename vector<M>::type;

template <class M, index_t<M> N, typename T>
using get_vector_t = typename vector<M>::template other_type<T, N>;

template <class M>
struct matrix {};

template <class M, index_t<M> ROWS, index_t<M> COLS, typename T>
using get_matrix_t = typename matrix<M>::template other_type<T, ROWS, COLS>;
/// @}

/// Matrix dimensions
/// @{
template <typename M>
struct dimensions {

  using size_type = int;

  // Error case
  static constexpr size_type dim{-1};
  static constexpr size_type rows{-1};
  static constexpr size_type columns{-1};
};

/// Specilization for scalar types
template <typename M>
requires std::is_fundamental_v<M> struct dimensions<M> {

  using size_type = std::size_t;

  static constexpr size_type dim{0};
  static constexpr size_type rows{1};
  static constexpr size_type columns{1};
};

template <class M>
inline constexpr index_t<M> dim{dimensions<std::remove_cvref_t<M>>::sim};

template <class M>
inline constexpr index_t<M> rows{dimensions<std::remove_cvref_t<M>>::rows};

template <class M>
inline constexpr index_t<M> columns{
    dimensions<std::remove_cvref_t<M>>::columns};

template <class M>
inline constexpr index_t<M> rank{std::min(rows<M>, columns<M>)};

template <class M>
inline constexpr index_t<M> size{rows<M> * columns<M>};

template <class V>
inline constexpr bool is_vector{dimensions<std::remove_cvref_t<V>>::dim == 1};

template <class M>
inline constexpr bool is_matrix{dimensions<std::remove_cvref_t<M>>::dim == 2};

template <class M>
inline constexpr bool is_square{(rows<M> == columns<M>)};
/// @}

/// Getter types
/// @{
template <class M>
struct element_getter {};

template <class M>
using element_getter_t = typename element_getter<M>::type;

template <class M>
struct block_getter {};

template <class M>
using block_getter_t = typename block_getter<M>::type;
/// @}

/// The algebra types
/// @{
template <typename T>
struct get_algebra {};

template <typename T>
requires(!std::is_same_v<typename T::point3D, void>) struct get_algebra<T> {
  template <typename U>
  using simd = typename T::template simd<U>;
  using size_type = typename T::size_type;
  using boolean = typename T::boolean;
  using value = typename T::value_type;
  using scalar = typename T::scalar;
  using point2D = typename T::point2D;
  using point3D = typename T::point3D;
  using vector3D = typename T::vector3D;
  using transform3D = typename T::transform3D;
  template <std::size_t ROWS, std::size_t COLS>
  using matrix = typename T::template matrix<ROWS, COLS>;
};

}  // namespace traits

template <typename A>
using get_value_t = typename traits::get_algebra<A>::value;

template <typename A>
using get_boolean_t = typename traits::get_algebra<A>::boolean;

template <typename A, typename T>
using get_simd_t = typename traits::get_algebra<A>::template simd<T>;

template <typename A>
using get_size_t = typename traits::get_algebra<A>::size_type;

template <typename A>
using get_scalar_t = typename traits::get_algebra<A>::scalar;

template <typename A>
using get_point2D_t = typename traits::get_algebra<A>::point2D;

template <typename A>
using get_point3D_t = typename traits::get_algebra<A>::point3D;

template <typename A>
using get_vector3D_t = typename traits::get_algebra<A>::vector3D;

template <typename A>
using get_transform3D_t = typename traits::get_algebra<A>::transform3D;

template <typename A, std::size_t R, std::size_t C>
using get_matrix_t = typename traits::get_algebra<A>::template matrix<R, C>;
/// @}

}  // namespace algebra

/// Default type trait specializations
/// @{
#define ALGEBRA_PLUGINS_DEFINE_TYPE_TRAITS(A)                           \
                                                                        \
  namespace traits {                                                    \
                                                                        \
  template <typename T, auto N>                                         \
  struct index<A::vector_type<T, N>> {                                  \
    using type = algebra::A::size_type;                                 \
  };                                                                    \
                                                                        \
  template <typename T, auto ROWS, auto COLS>                           \
  struct index<A::matrix_type<T, ROWS, COLS>> {                         \
    using type = algebra::A::size_type;                                 \
  };                                                                    \
                                                                        \
  template <typename T, auto N>                                         \
  struct dimensions<A::vector_type<T, N>> {                             \
                                                                        \
    using size_type = index_t<A::vector_type<T, N>>;                    \
                                                                        \
    static constexpr size_type dim{1};                                  \
    static constexpr size_type rows{N};                                 \
    static constexpr size_type columns{1};                              \
  };                                                                    \
                                                                        \
  template <typename T, auto ROWS, auto COLS>                           \
  struct dimensions<A::matrix_type<T, ROWS, COLS>> {                    \
                                                                        \
    using size_type = index_t<A::matrix_type<T, ROWS, COLS>>;           \
                                                                        \
    static constexpr size_type dim{2};                                  \
    static constexpr size_type rows{ROWS};                              \
    static constexpr size_type columns{COLS};                           \
  };                                                                    \
                                                                        \
  template <typename T, auto N>                                         \
  struct value<A::vector_type<T, N>> {                                  \
    using type = T;                                                     \
  };                                                                    \
                                                                        \
  template <typename T, auto ROWS, auto COLS>                           \
  struct value<A::matrix_type<T, ROWS, COLS>> {                         \
    using type = T;                                                     \
  };                                                                    \
                                                                        \
  template <typename T, auto N>                                         \
  struct vector<A::vector_type<T, N>> {                                 \
                                                                        \
    template <typename other_T, auto other_N>                           \
    using other_type = A::vector_type<other_T, other_N>;                \
                                                                        \
    using type = other_type<T, N>;                                      \
  };                                                                    \
                                                                        \
  template <typename T, auto ROWS, auto COLS>                           \
  struct vector<A::matrix_type<T, ROWS, COLS>> {                        \
                                                                        \
    template <typename other_T, auto other_N>                           \
    using other_type = A::vector_type<other_T, other_N>;                \
                                                                        \
    using type = other_type<T, ROWS>;                                   \
  };                                                                    \
                                                                        \
  template <typename T, auto ROWS, auto COLS>                           \
  struct matrix<A::matrix_type<T, ROWS, COLS>> {                        \
    template <typename other_T, auto other_ROWS, auto other_COLS>       \
    using other_type = A::matrix_type<other_T, other_ROWS, other_COLS>; \
                                                                        \
    using type = A::matrix_type<T, ROWS, COLS>;                         \
  };                                                                    \
                                                                        \
  template <typename T, auto N>                                         \
  struct matrix<A::vector_type<T, N>> {                                 \
    template <typename other_T, auto other_ROWS, auto other_COLS>       \
    using other_type = A::matrix_type<other_T, other_ROWS, other_COLS>; \
                                                                        \
    using type = other_type<T, N, 1>;                                   \
  };                                                                    \
                                                                        \
  template <typename T, auto N>                                         \
  struct element_getter<A::vector_type<T, N>> {                         \
    using type = A::element_getter;                                     \
  };                                                                    \
                                                                        \
  template <typename T, auto ROWS, auto COLS>                           \
  struct element_getter<A::matrix_type<T, ROWS, COLS>> {                \
    using type = A::element_getter;                                     \
  };                                                                    \
                                                                        \
  template <typename T, auto ROWS, auto COLS>                           \
  struct block_getter<A::matrix_type<T, ROWS, COLS>> {                  \
    using type = A::block_getter;                                       \
  };                                                                    \
                                                                        \
  }  // namespace algebra::traits
