#include "Dynamics/OrbitalDynamics.h"
#include <Eigen/Dense>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Two-body acceleration is correct", "[OrbitalDynamics]")
{
    constexpr double mu = 3.986004418e14;

    const Eigen::Vector3d position(7.0e6, 0.0, 0.0);
    const Eigen::Vector3d velocity(0.0, 7.5e3, 0.0);

    const StateVector state(position, velocity);

    const OrbitalDynamics dynamics;

    const StateVector derivative = dynamics.derivative(state);

    const Eigen::Vector3d expectedAcceleration(-mu / (7.0e6 * 7.0e6), 0.0, 0.0);

    REQUIRE(derivative.getPosition() == velocity);
    REQUIRE( derivative.getVelocity().isApprox(expectedAcceleration, 1.0e-12));
}