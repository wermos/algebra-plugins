/** Algebra plugins library, part of the ACTS project
 *
 * (c) 2023-2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

// Project include(s)
#include "algebra/array_cmath.hpp"
#include "benchmark/array/data_generator.hpp"
#include "benchmark/common/benchmark_vector.hpp"

// Benchmark include
#include <benchmark/benchmark.h>

// System include(s)
#include <string>

using namespace algebra;

/// Run vector benchmarks
int main(int argc, char** argv) {

  //
  // Prepare benchmarks
  //
  algebra::benchmark_base::configuration cfg{};
  cfg.n_samples(100000);

  using add_f_t = vector_binaryOP_bm<array::vector3, float, bench_op::add>;
  using sub_f_t = vector_binaryOP_bm<array::vector3, float, bench_op::sub>;
  using dot_f_t = vector_binaryOP_bm<array::vector3, float, bench_op::dot>;
  using cross_f_t = vector_binaryOP_bm<array::vector3, float, bench_op::cross>;
  using normlz_f_t =
      vector_unaryOP_bm<array::vector3, float, bench_op::normalize>;

  using add_d_t = vector_binaryOP_bm<array::vector3, double, bench_op::add>;
  using sub_d_t = vector_binaryOP_bm<array::vector3, double, bench_op::sub>;
  using dot_d_t = vector_binaryOP_bm<array::vector3, double, bench_op::dot>;
  using cross_d_t = vector_binaryOP_bm<array::vector3, double, bench_op::cross>;
  using normlz_d_t =
      vector_unaryOP_bm<array::vector3, double, bench_op::normalize>;

  std::cout << "-----------------------------------------------\n"
            << "Algebra-Plugins 'vector' benchmark (std::array)\n"
            << "-----------------------------------------------\n\n"
            << cfg;

  //
  // Register all benchmarks
  //
  ALGEBRA_PLUGINS_REGISTER_VECTOR_BENCH(cfg)

  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
}
