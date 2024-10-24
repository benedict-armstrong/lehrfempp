/**
 * @file
 * @brief Test if the lf::uscalfe::isMeshFunction works as expected.
 * @author Raffael Casagrande
 * @date   2018-12-15 04:12:13
 * @copyright MIT License
 */

#include <gtest/gtest.h>
#include <lf/mesh/utils/utils.h>

namespace lf::mesh::utils::test {

static_assert(MeshFunction<MeshFunctionAT<double>>);
static_assert(MeshFunction<MeshFunctionAT<double>, double>);

struct ProperMeshFunction {
  auto operator()(const mesh::Entity& e, const Eigen::MatrixXd& local) const {
    return std::vector<double>{1.0};
  }
};

struct NotCopyableMeshFunction {
  NotCopyableMeshFunction(const NotCopyableMeshFunction&) = delete;

  auto operator()(const mesh::Entity& e, const Eigen::MatrixXd& local) const {
    return std::vector<double>{1.0};
  }
};

struct NotMoveableMeshFunction {
  NotMoveableMeshFunction(const NotMoveableMeshFunction&) = delete;

  auto operator()(const mesh::Entity& e, const Eigen::MatrixXd& local) const {
    return std::vector<double>{1.0};
  }
};

struct NotCallableMeshFunction {};

TEST(isMeshFunctionTestSuite, checkIsMeshFunction) {
  EXPECT_TRUE(MeshFunction<ProperMeshFunction>);
  EXPECT_TRUE((MeshFunction<ProperMeshFunction, double>));
  EXPECT_FALSE((MeshFunction<ProperMeshFunction, float>));
  EXPECT_TRUE(MeshFunction<const ProperMeshFunction>);
  EXPECT_TRUE((MeshFunction<const ProperMeshFunction, double>));
  EXPECT_FALSE((MeshFunction<const ProperMeshFunction, std::vector<double>>));
  EXPECT_TRUE(
      (std::is_same_v<MeshFunctionReturnType<ProperMeshFunction>, double>));

  // references are not MeshFunctions
  EXPECT_FALSE(MeshFunction<ProperMeshFunction&>);
  EXPECT_FALSE((MeshFunction<ProperMeshFunction&, double>));
  EXPECT_FALSE(MeshFunction<const ProperMeshFunction&>);
  EXPECT_FALSE((MeshFunction<const ProperMeshFunction&, double>));
  EXPECT_FALSE(MeshFunction<ProperMeshFunction&&>);
  EXPECT_FALSE((MeshFunction<ProperMeshFunction&&, double>));

  // also pointers don't work
  EXPECT_FALSE(MeshFunction<ProperMeshFunction*>);

  // check with lambda
  auto lambda = [](const mesh::Entity& e, const Eigen::MatrixXd& local) {
    return std::vector<double>{1.0};
  };
  EXPECT_TRUE(MeshFunction<decltype(lambda)>);
  EXPECT_TRUE((MeshFunction<decltype(lambda), double>));
  EXPECT_TRUE(
      (std::is_same_v<MeshFunctionReturnType<decltype(lambda)>, double>));

  // test that any of the other expressions don't work
  EXPECT_FALSE(MeshFunction<NotCopyableMeshFunction>);
  EXPECT_FALSE((MeshFunction<NotCopyableMeshFunction, double>));
  EXPECT_FALSE(MeshFunction<NotMoveableMeshFunction>);
  EXPECT_FALSE((MeshFunction<NotMoveableMeshFunction, double>));
  EXPECT_FALSE(MeshFunction<NotCallableMeshFunction>);
  EXPECT_FALSE((MeshFunction<NotCallableMeshFunction, double>));

  // if operator() expects wrong number of arguments
  auto l2 = [](const mesh::Entity& e) { return std::vector{1.0}; };
  EXPECT_FALSE(MeshFunction<decltype(l2)>);
  EXPECT_FALSE((MeshFunction<decltype(l2), double>));

  // if operator() has a template argument
  auto l3 = [](auto& e, auto local) { return std::vector{1.0}; };
  EXPECT_TRUE(MeshFunction<decltype(l3)>);
  EXPECT_TRUE((MeshFunction<decltype(l3), double>));
  EXPECT_FALSE((MeshFunction<decltype(l3), float>));
  EXPECT_TRUE((std::is_same_v<MeshFunctionReturnType<decltype(l3)>, double>));

  // if operator() expects std::vector instead of Eigen::MatrixXd
  auto l4 = [](const mesh::Entity&, std::vector<double> local) {
    return std::vector{1.0};
  };
  EXPECT_FALSE(MeshFunction<decltype(l4)>);
  EXPECT_FALSE((MeshFunction<decltype(l4), double>));

  // if operator() doesn't return std::vector
  auto l5 = [](const mesh::Entity&, Eigen::MatrixXd) { return 1; };
  EXPECT_FALSE(MeshFunction<decltype(l5)>);
  EXPECT_FALSE((MeshFunction<decltype(l5), int>));

  // if operator() returns a vector of 3x1 eigen vectors
  auto l6 = [](const mesh::Entity&, const Eigen::MatrixXd&) {
    return std::vector{Eigen::Vector3d(1.0, 1.0, 1.0)};
  };
  EXPECT_TRUE(MeshFunction<decltype(l6)>);
  EXPECT_TRUE((MeshFunction<decltype(l6), Eigen::Vector3d>));
  EXPECT_TRUE(
      (std::is_same_v<MeshFunctionReturnType<decltype(l6)>, Eigen::Vector3d>));

  // if operator() accepts Eigen matrix by value
  auto l7 = [](const mesh::Entity&, Eigen::MatrixXd) {
    return std::vector{1.0};
  };
  EXPECT_TRUE(MeshFunction<decltype(l7)>);
  EXPECT_TRUE((MeshFunction<decltype(l7), double>));
  EXPECT_TRUE((std::is_same_v<MeshFunctionReturnType<decltype(l7)>, double>));

  // if operator() accepts Eigen matrix by mutable reference
  auto l8 = [](const mesh::Entity&, Eigen::MatrixXd&) {
    return std::vector{1.0};
  };
  EXPECT_FALSE(MeshFunction<decltype(l8)>);
  EXPECT_FALSE((MeshFunction<decltype(l8), double>));
}

}  // namespace lf::mesh::utils::test
