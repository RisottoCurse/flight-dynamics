#include <fstream>
#include <iostream>

#include "Eigen/Core"
#include "Common/Constants.h"
#include "State/State.h"
#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"

int J2Dynamics(const double mu, StateVector& initial);

int main()
{
    // Keplerian Elements for Circular Orbit
    const constexpr double a{7000e3}; // 7000 km
    const constexpr double inclination{60.0 * (MathsConstants::pi / 180)}; // converting 60 degrees to radians

    const double velocityMagnitude = std::sqrt(EarthConstants::MU / a);  // sqrt (GM/r)

    Eigen::Vector3d position(a, 0.0, 0.0);

    Eigen::Vector3d velocity(0.0, 
                            velocityMagnitude * std::cos(inclination), 
                            velocityMagnitude * std::sin(inclination)
                        );

    StateVector initialState(position, velocity);

    int j2dynam = J2Dynamics(EarthConstants::MU, initialState);

    return 0;
}

int J2Dynamics(const double mu, StateVector& initial) {

    OrbitalDynamics j2Dynamics;

    j2Dynamics.enableJ2(true);

    Propagator propagator(j2Dynamics, 1); // 1 second timestep

    const double duration = 86400; // 1 day


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
           << state.getVelocity().z() << "\n";
        time += propagator.getStepSize();
    }

    output.close();

    return 0;

}
