#include <fstream>
#include <iostream>

#include "Common/Constants.h"
#include "Eigen/Core"
#include "State/State.h"
#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"

int TwoBodyDynamics(double mu, StateVector& initial);

int main()
{
    const double radius = EarthConstants::RADIUS + EarthConstants::LEOAltitude; // m

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

    int twoBody = TwoBodyDynamics(EarthConstants::MU, initialState);

    return 0;
}


int TwoBodyDynamics(const double mu, StateVector& initial) {

    OrbitalDynamics twoBody;

    Propagator propagator(twoBody, 1); // 1 second timestep

    const double duration = 86400; // 1 day

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