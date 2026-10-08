#include <fstream>
#include <iostream>

#include "Eigen/Core"
#include "State/State.h"
#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"

int TwoBodyDynamics(double mu, StateVector& initial);
int J2Dynamics(const double mu, StateVector& initial);

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

    int twoBody = TwoBodyDynamics(MU_EARTH, initialState);
    int j2dynam = J2Dynamics(MU_EARTH, initialState);

    return 0;
}


int TwoBodyDynamics(const double mu, StateVector& initial) {

    OrbitalDynamics twoBody;

    Propagator propagator(twoBody, 1); // 1 second timestep

    const double duration = 6564.81; // T = 2 * pi sqrt(r^3/GM) 

    std::vector<StateVector> trajectory = propagator.propagate(initial, duration);

    std::ofstream output("notebook/2BodyTests/orbit_trajectory.csv");
    
    if (!output) {
        std::cerr << "Failed to open 2-body orbit_trajectory.csv\n";
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


int J2Dynamics(const double mu, StateVector& initial) {

    OrbitalDynamics j2Dynamics;

    j2Dynamics.enableJ2(true);

    Propagator propagator(j2Dynamics, 1); // 1 second timestep

    const double duration = 6564.81; // T = 2 * pi sqrt(r^3/GM) 

    std::vector<StateVector> trajectory = propagator.propagate(initial, duration);

    std::ofstream output("notebook/J2Tests/j2_orbit_trajectory.csv");
    
    if (!output) {
        std::cerr << "Failed to open j2 orbit_trajectory.csv\n";
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