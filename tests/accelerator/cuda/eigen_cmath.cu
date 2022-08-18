/** Algebra plugins library, part of the ACTS project
 *
 * (c) 2020-2022 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

// Project include(s).
#include "algebra/eigen_cmath.hpp"

// Local include(s).
#include "test_cuda_basics.cuh"

// GoogleTest include(s).
#include <gtest/gtest.h>

// System include(s).
#include <string>

/// Struct providing a readable name for the test
struct test_specialisation_name {
  template <typename T>
  static std::string GetName(int i) {
    switch (i) {
      case 0:
        return "cuda_eigen_cmath<float>";
      case 1:
        return "cuda_eigen_cmath<double>";
      default:
        return "unknown";
    }
  }
};

// Instantiate the test(s).
typedef testing::Types<
    test_types<
        float, algebra::eigen::point2<float>, algebra::eigen::point3<float>,
        algebra::eigen::vector2<float>, algebra::eigen::vector3<float>,
        algebra::eigen::transform3<float>, algebra::eigen::cartesian2<float>,
        algebra::eigen::cartesian3<float>, algebra::eigen::polar2<float>,
        algebra::eigen::cylindrical2<float>,
        algebra::eigen::cylindrical3<float>, algebra::eigen::line2<float>,
        algebra::eigen::bound_track_parameters<float>,
        algebra::eigen::free_track_parameters<float>,
        algebra::eigen::column_wise_operator<float>, int,
        algebra::eigen::matrix_type, algebra::vector::actor,
        algebra::matrix::actor<float,
                               algebra::matrix::determinant::preset0<float>,
                               algebra::matrix::inverse::preset0<float>>>,
    test_types<
        double, algebra::eigen::point2<double>, algebra::eigen::point3<double>,
        algebra::eigen::vector2<double>, algebra::eigen::vector3<double>,
        algebra::eigen::transform3<double>, algebra::eigen::cartesian2<double>,
        algebra::eigen::cartesian3<double>, algebra::eigen::polar2<double>,
        algebra::eigen::cylindrical2<double>,
        algebra::eigen::cylindrical3<double>, algebra::eigen::line2<double>,
        algebra::eigen::bound_track_parameters<double>,
        algebra::eigen::free_track_parameters<double>,
        algebra::eigen::column_wise_operator<double>, int,
        algebra::eigen::matrix_type, algebra::vector::actor,
        algebra::matrix::actor<double,
                               algebra::matrix::determinant::preset0<double>,
                               algebra::matrix::inverse::preset0<double>>>>
    eigen_cmath_types;
INSTANTIATE_TYPED_TEST_SUITE_P(algebra_plugins, test_cuda_basics,
                               eigen_cmath_types, test_specialisation_name);
