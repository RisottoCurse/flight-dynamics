#include <fstream>
#include <iostream>

#include "Eigen/Core"
#include "State/State.h"
#include "Dynamics/TwoBodyDynamics.h"
#include "Propagator/Propagator.h"


int main()
{
    constexpr double MU_EARTH = 3.986004418e14; // m^3/s^2
    constexpr double EARTH_RADIUS = 6378.137e3; // m

    const double altitude = 1200e3; // m (1200 km for LEO)
    const double radius = EARTH_RADIUS + altitude; // m

    Eigen::Vector3d position(radius, 0.0, 0.0);

    const double circularVelocity = std::sqrt(MU_EARTH / radius);

    Eigen::Vector3d velocity(0.0, circularVelocity, 0.0);

    StateVector initialState(position, velocity);

    TwoBodyDynamics dynamics(MU_EARTH);

    Propagator propagator(dynamics, 1); // 1 second timestep

    const double duration = 6564.81; // T = 2 * pi sqrt(r^3/GM) 

    std::vector<StateVector> trajectory = propagator.propagate(initialState, duration);

    std::ofstream output("notebook/orbit_trajectory.csv");
    
    if (!output) {
        std::cerr << "Failed to open orbit_trajectory.csv\n";
        return 1;
}

    output << "time,x,y,z,u,v,w\n";

    double time{0};

    for (const StateVector& state : trajectory) {
        output << std::setprecision(15)
           << time << ","
           << state.getPosition().x() << ","
           << state.getPosition().y() << ","
           << state.getPosition().z() << ","
           << state.getVelocity().x() << ","
           << state.getVelocity().y() << ","
           << state.getPosition().z() << "\n";
        time += propagator.getStepSize();
    }

    output.close();

    return 0;
}
