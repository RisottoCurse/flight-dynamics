#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"
#include "State/State.h"
#include <Eigen/Dense>
#include <catch2/catch_test_macros.hpp>
#include <numbers>

TEST_CASE("Circular orbit returns to initial position with 2 body dynamics", "[Propagator]")
{
    constexpr double mu = 3.986004418e14;
    constexpr double radius = 7.0e6;

    const double velocity = std::sqrt(mu / radius);

    const double period = 2.0 * std::numbers::pi * std::sqrt((radius*radius*radius)/mu);

    const Eigen::Vector3d positionVector(radius, 0.0, 0.0);

    const Eigen::Vector3d velocityVector(0.0, velocity, 0.0);

    const StateVector initialState(positionVector,velocityVector);

    // Propagate for one orbital period
    OrbitalDynamics TwoBodyDynamics;

    constexpr double stepSize = 1;

    Propagator propagator(TwoBodyDynamics, stepSize);

    std::vector<StateVector> result = propagator.propagate(initialState,  period);

    const StateVector finalState = result.back();

    REQUIRE(finalState.getPosition().isApprox(initialState.getPosition(), 1e-11));
    REQUIRE(finalState.getVelocity().isApprox(initialState.getVelocity(), 1e-11));
}