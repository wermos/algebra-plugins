/** Algebra plugins library, part of the ACTS project
 *
 * (c) 2020-2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s).
#include "algebra/qualifiers.hpp"
#include "algebra/storage/impl/eigen_array.hpp"
#include "algebra/utils/approximately_equal.hpp"

// Eigen include(s).
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif  // MSVC
#ifdef __NVCC_DIAG_PRAGMA_SUPPORT__
#pragma nv_diagnostic push
#pragma nv_diag_suppress 20011
#pragma nv_diag_suppress 20012
#pragma nv_diag_suppress 20014
#endif  // __NVCC_DIAG_PRAGMA_SUPPORT__
#include <Eigen/Core>
#include <Eigen/Geometry>
#ifdef _MSC_VER
#pragma warning(pop)
#endif  // MSVC
#ifdef __NVCC_DIAG_PRAGMA_SUPPORT__
#pragma nv_diagnostic pop
#endif  // __NVCC_DIAG_PRAGMA_SUPPORT__

// System include(s).
#include <cassert>
#include <concepts>
#include <limits>
#include <type_traits>

namespace algebra::eigen::math {

/// Transform wrapper class to ensure standard API within differnt plugins
template <concepts::scalar scalar_t>
struct transform3 {

  /// @name Type definitions for the struct
  /// @{

  /// Scalar type used by the transform
  using scalar_type = scalar_t;

  /// Array type used by the transform
  template <int N>
  using array_type = eigen::array<scalar_type, N>;

  /// 3-element "vector" type
  using vector3 = array_type<3>;
  /// Point in 3D space
  using point3 = vector3;
  /// Point in 2D space
  using point2 = array_type<2>;

  /// 4x4 matrix type
  using matrix44 =
      typename Eigen::Transform<scalar_type, 3, Eigen::Affine>::MatrixType;

  /// Helper type to cast this to another floating point precision
  template <concepts::scalar o_scalar_t>
  using other_type = transform3<o_scalar_t>;

  /// @}

  /// @name Data objects
  /// @{

  Eigen::Transform<scalar_type, 3, Eigen::Affine> _data;
  Eigen::Transform<scalar_type, 3, Eigen::Affine> _data_inv;

  /// @}

  /// Contructor with arguments: t, x, y, z
  ///
  /// @param t the translation (or origin of the new frame)
  /// @param x the x axis of the new frame
  /// @param y the y axis of the new frame
  /// @param z the z axis of the new frame, normal vector for planes
  ALGEBRA_HOST_DEVICE
  transform3(const vector3 &t, const vector3 &x, const vector3 &y,
             const vector3 &z, bool get_inverse = true) {
    _data.setIdentity();

    auto &matrix = _data.matrix();
    matrix.template block<3, 1>(0, 0) = x;
    matrix.template block<3, 1>(0, 1) = y;
    matrix.template block<3, 1>(0, 2) = z;
    matrix.template block<3, 1>(0, 3) = t;

    if (get_inverse) {
      _data_inv = _data.inverse();
    } else {
      _data_inv.setIdentity();
    }
  }

  /// Contructor with arguments: t, z, x
  ///
  /// @param t the translation (or origin of the new frame)
  /// @param z the z axis of the new frame, normal vector for planes
  /// @param x the x axis of the new frame
  ALGEBRA_HOST_DEVICE
  transform3(const vector3 &t, const vector3 &z, const vector3 &x,
             bool get_inverse = true)
      : transform3(t, x, z.cross(x), z, get_inverse) {}

  /// Constructor with arguments: translation
  ///
  /// @param t is the transform
  ALGEBRA_HOST_DEVICE
  explicit transform3(const vector3 &t) {
    _data.setIdentity();

    auto &matrix = _data.matrix();
    matrix.template block<3, 1>(0, 3) = t;

    _data_inv = _data.inverse();
  }

  /// Constructor with arguments: matrix
  ///
  /// @param m is the full 4x4 matrix
  ALGEBRA_HOST_DEVICE
  explicit transform3(const matrix44 &m) {

    _data.matrix() = m;

    _data_inv = _data.inverse();
  }

  /// Constructor with arguments: matrix and its inverse
  ///
  /// @param m is the full 4x4 matrix
  /// @param m_inv is the inverse to m
  ALGEBRA_HOST_DEVICE
  transform3(const matrix44 &m, const matrix44 &m_inv)
      : _data{m}, _data_inv{m_inv} {
    // The assertion will not hold for (casts to) int
    if constexpr (std::floating_point<scalar_type>) {
      [[maybe_unused]] constexpr auto epsilon{
          std::numeric_limits<scalar_type>::epsilon()};
      assert(algebra::approx_equal(matrix44(m * m_inv),
                                   matrix44(matrix44::Identity()),
                                   16.f * epsilon, 1e-6f));
    }
  }

  /// Constructor with arguments: matrix as std::aray of scalar
  ///
  /// @param ma is the full 4x4 matrix as a 16 array
  ALGEBRA_HOST_DEVICE
  explicit transform3(const array_type<16> &ma) {

    _data.matrix() << ma[0], ma[1], ma[2], ma[3], ma[4], ma[5], ma[6], ma[7],
        ma[8], ma[9], ma[10], ma[11], ma[12], ma[13], ma[14], ma[15];

    _data_inv = _data.inverse();
  }

  /// Default constructor: set contents to identity matrices
  ALGEBRA_HOST_DEVICE
  transform3() {
    _data.setIdentity();
    _data_inv.setIdentity();
  }

  /// Default contructors
  transform3(const transform3 &rhs) = default;
  ~transform3() = default;

  /// Equality operator
  ALGEBRA_HOST_DEVICE
  constexpr bool operator==(const transform3 &rhs) const {
    return (_data.isApprox(rhs._data));
  }

  /// Rotate a vector into / from a frame
  ///
  /// @param m is the rotation matrix
  /// @param v is the vector to be rotated
  template <typename derived_type>
  requires(Eigen::MatrixBase<derived_type>::RowsAtCompileTime == 3 &&
           Eigen::MatrixBase<derived_type>::ColsAtCompileTime ==
               1) ALGEBRA_HOST_DEVICE
      static constexpr auto rotate(
          const Eigen::Transform<scalar_type, 3, Eigen::Affine> &m,
          const Eigen::MatrixBase<derived_type> &v) {
    return m.matrix().template block<3, 3>(0, 0) * v;
  }

  /// This method retrieves the rotation of a transform
  ALGEBRA_HOST_DEVICE
  constexpr auto rotation() const {
    return _data.matrix().template block<3, 3>(0, 0);
  }

  /// This method retrieves x axis
  ALGEBRA_HOST_DEVICE
  constexpr point3 x() const {
    return _data.matrix().template block<3, 1>(0, 0);
  }

  /// This method retrieves y axis
  ALGEBRA_HOST_DEVICE
  constexpr point3 y() const {
    return _data.matrix().template block<3, 1>(0, 1);
  }

  /// This method retrieves z axis
  ALGEBRA_HOST_DEVICE
  constexpr point3 z() const {
    return _data.matrix().template block<3, 1>(0, 2);
  }

  /// This method retrieves the translation of a transform
  ALGEBRA_HOST_DEVICE
  constexpr point3 translation() const {
    return _data.matrix().template block<3, 1>(0, 3);
  }

  /// This method retrieves the 4x4 matrix of a transform
  ALGEBRA_HOST_DEVICE
  constexpr const matrix44 &matrix() const { return _data.matrix(); }

  /// This method retrieves the 4x4 matrix of an inverse transform
  ALGEBRA_HOST_DEVICE
  constexpr const matrix44 &matrix_inverse() const {
    return _data_inv.matrix();
  }

  /// This method transform from a point from the local 3D cartesian frame to
  /// the global 3D cartesian frame
  template <typename derived_type>
  requires(Eigen::MatrixBase<derived_type>::RowsAtCompileTime == 3 &&
           Eigen::MatrixBase<derived_type>::ColsAtCompileTime ==
               1) ALGEBRA_HOST_DEVICE
      constexpr auto point_to_global(
          const Eigen::MatrixBase<derived_type> &v) const {
    return (_data * v);
  }

  /// This method transform from a vector from the global 3D cartesian frame
  /// into the local 3D cartesian frame
  template <typename derived_type>
  requires(Eigen::MatrixBase<derived_type>::RowsAtCompileTime == 3 &&
           Eigen::MatrixBase<derived_type>::ColsAtCompileTime ==
               1) ALGEBRA_HOST_DEVICE
      constexpr auto point_to_local(
          const Eigen::MatrixBase<derived_type> &v) const {
    return (_data_inv * v);
  }

  /// This method transform from a vector from the local 3D cartesian frame to
  /// the global 3D cartesian frame
  template <typename derived_type>
  requires(Eigen::MatrixBase<derived_type>::RowsAtCompileTime == 3 &&
           Eigen::MatrixBase<derived_type>::ColsAtCompileTime ==
               1) ALGEBRA_HOST_DEVICE
      constexpr auto vector_to_global(
          const Eigen::MatrixBase<derived_type> &v) const {
    return (_data.linear() * v);
  }

  /// This method transform from a vector from the global 3D cartesian frame
  /// into the local 3D cartesian frame
  template <typename derived_type>
  requires(Eigen::MatrixBase<derived_type>::RowsAtCompileTime == 3 &&
           Eigen::MatrixBase<derived_type>::ColsAtCompileTime ==
               1) ALGEBRA_HOST_DEVICE
      constexpr auto vector_to_local(
          const Eigen::MatrixBase<derived_type> &v) const {
    return (_data_inv.linear() * v);
  }
};  // struct transform3

}  // namespace algebra::eigen::math
