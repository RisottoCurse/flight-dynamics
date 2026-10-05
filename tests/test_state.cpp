#include <catch2/catch_test_macros.hpp>
#include "State/State.h"
#include <Eigen/Dense>

TEST_CASE("StateVector stores position and velocity", "[StateVector]")
{
    const Eigen::Vector3d position(1.0, 2.0, 3.0);
    const Eigen::Vector3d velocity(4.0, 5.0, 6.0);

    const StateVector state(position, velocity);

    REQUIRE(state.getPosition() == position);
    REQUIRE(state.getVelocity() == velocity);
}