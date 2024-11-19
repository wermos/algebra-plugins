/** Algebra plugins library, part of the ACTS project
 *
 * (c) 2020-2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s).
#include "algebra/math/common.hpp"
#include "algebra/math/impl/vc_aos_vector.hpp"
#include "algebra/qualifiers.hpp"
#include "algebra/storage/matrix.hpp"
#include "algebra/storage/vector.hpp"

// Vc include(s).
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif  // MSVC
#include <Vc/Vc>
#ifdef _MSC_VER
#pragma warning(pop)
#endif  // MSVC

namespace algebra::vc_aos::math {

/// This method retrieves phi from a vector, vector base with rows >= 2
///
/// @param v the input vector
template <typename vector_t>
requires(Vc::is_simd_vector<vector_t>::value ||
         algebra::detail::is_storage_vector_v<vector_t>) ALGEBRA_HOST_DEVICE
    inline auto phi(const vector_t &v) noexcept {

  return algebra::math::atan2(v[1], v[0]);
}

/// This method retrieves the perpendicular magnitude of a vector with rows >= 2
///
/// @param v the input vector
template <typename vector_t>
requires(Vc::is_simd_vector<vector_t>::value ||
         algebra::detail::is_storage_vector_v<vector_t>) ALGEBRA_HOST_DEVICE
    inline auto perp(const vector_t &v) noexcept {

  return algebra::math::sqrt(v[0] * v[0] + v[1] * v[1]);
}

/// This method retrieves theta from a vector, vector base with rows >= 3
///
/// @param v the input vector
template <typename vector_t>
requires(Vc::is_simd_vector<vector_t>::value ||
         algebra::detail::is_storage_vector_v<vector_t>) ALGEBRA_HOST_DEVICE
    inline auto theta(const vector_t &v) noexcept {

  return algebra::math::atan2(perp(v), v[2]);
}

/// This method retrieves the norm of a vector, no dimension restriction
///
/// @param v the input vector
template <typename vector_t>
requires(Vc::is_simd_vector<vector_t>::value ||
         algebra::detail::is_storage_vector_v<vector_t>) ALGEBRA_HOST_DEVICE
    inline auto norm(const vector_t &v) {

  return algebra::math::sqrt(dot(v, v));
}

/// This method retrieves the pseudo-rapidity from a vector or vector base with
/// rows >= 3
///
/// @param v the input vector
template <typename vector_t>
requires(Vc::is_simd_vector<vector_t>::value ||
         algebra::detail::is_storage_vector_v<vector_t>) ALGEBRA_HOST_DEVICE
    inline auto eta(const vector_t &v) noexcept {

  return algebra::math::atanh(v[2] / norm(v));
}

/// Function extracting an element from a matrix (const)
template <typename matrix_t>
requires(Vc::is_simd_vector<typename matrix_t::vector_type>::value ||
         algebra::detail::is_storage_vector_v<typename matrix_t::vector_type>)
    ALGEBRA_HOST_DEVICE inline decltype(auto)
        element(const matrix_t &m, std::size_t row, std::size_t col) {

  return m[col][row];
}

/// Function extracting an element from a matrix (non-const)
template <typename matrix_t>
requires(Vc::is_simd_vector<typename matrix_t::vector_type>::value ||
         algebra::detail::is_storage_vector_v<typename matrix_t::vector_type>)
    ALGEBRA_HOST_DEVICE inline decltype(auto)
        element(matrix_t &m, std::size_t row, std::size_t col) {

  return m[col][row];
}

}  // namespace algebra::vc_aos::math
