/** Algebra plugins library, part of the ACTS project
 *
 * (c) 2020-2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

// Project include(s).
#include "algebra/smatrix_generic.hpp"

// Test include(s).
#include "test_host_basics.hpp"

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
        return "smatrix_generic<float>";
      case 1:
        return "smatrix_generic<double>";
      default:
        return "unknown";
    }
  }
};

// Register the tests
REGISTER_TYPED_TEST_SUITE_P(test_host_basics_vector, local_vectors, vector3,
                            getter);
TEST_HOST_BASICS_MATRIX_TESTS();
REGISTER_TYPED_TEST_SUITE_P(test_host_basics_transform, transform3,
                            global_transformations);

// Instantiate the test(s).
typedef testing::Types<
    test_types<
        float, algebra::smatrix::point2<float>, algebra::smatrix::point3<float>,
        algebra::smatrix::vector2<float>, algebra::smatrix::vector3<float>,
        algebra::smatrix::transform3<float>, unsigned int,
        algebra::smatrix::matrix_type>,
    test_types<
        double, algebra::smatrix::point2<double>,
        algebra::smatrix::point3<double>, algebra::smatrix::vector2<double>,
        algebra::smatrix::vector3<double>, algebra::smatrix::transform3<double>,
        unsigned int, algebra::smatrix::matrix_type>>
    smatrix_generic_types;
INSTANTIATE_TYPED_TEST_SUITE_P(algebra_plugins, test_host_basics_vector,
                               smatrix_generic_types, test_specialisation_name);
INSTANTIATE_TYPED_TEST_SUITE_P(algebra_plugins, test_host_basics_matrix,
                               smatrix_generic_types, test_specialisation_name);
INSTANTIATE_TYPED_TEST_SUITE_P(algebra_plugins, test_host_basics_transform,
                               smatrix_generic_types, test_specialisation_name);
