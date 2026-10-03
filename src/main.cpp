#include <iostream>


#include "Eigen/Core"
#include "State/State.h"
#include "Dynamics/TwoBodyDynamics.h"
#include "Propagator/Propagator.h"


int main()
{
    constexpr double MU_EARTH = 3.986004418e14;
    constexpr double EARTH_RADIUS = 6378.137e3;

    const double altitude = 1200e3; // 1200 km for LEO
    const double radius = EARTH_RADIUS + altitude;

    Eigen::Vector3d position(radius, 0.0, 0.0);

    const double circularVelocity = std::sqrt(MU_EARTH / radius);

    Eigen::Vector3d velocity(0.0, circularVelocity, 0.0);

    StateVector initialState(position, velocity);

    TwoBodyDynamics dynamics(MU_EARTH);

    Propagator propagator(dynamics, 1); // 1 second timestep

    const double duration = 5400.0; // 90 minutes

    StateVector finalState = propagator.propagate(initialState, duration);

    std::cout << "Final position: "
              << finalState.getPosition()
              << "\n\n";

    std::cout << "Final velocity: "
              << finalState.getVelocity()
              << "\n";

    return 0;
}