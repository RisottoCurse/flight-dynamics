#include "Common/maths.h"
#include "Common/Constants.h"
#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"
#include "State/State.h"
#include "Manoeuvre/ImpulsiveManoeuvre.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {

    constexpr double dt{1.0};
    constexpr double duration{10800.0};
    constexpr double burnTime{1800.0};

    // initial circular orbit
    constexpr double a{7000e3}; // semi-major axis in meters
    constexpr double inclination = DEG2RAD * 60.0; // inclination in radians

    const double velocityMagnitude = std::sqrt(EarthConstants::MU / a);

    const Vector3 position(a, 0.0, 0.0); // position in ECI frame
    const Velocity velocity(0.0, velocityMagnitude * std::cos(inclination), velocityMagnitude * std::sin(inclination)); // velocity in ECI frame

    const StateVector initialState(position, velocity);

    // 2 body dynamics
    OrbitalDynamics dynamics(EarthConstants::MU);

    Propagator propagator(dynamics, dt);


    // Define an impulsive manoeuvre in RTN frame
    const Vector3 deltaV_RTN(0.0, 100.0, 0.0); // 100 m/s in the tangential direction

    const ImpulsiveManoeuvre manoeuvre(deltaV_RTN, ManoeuvreFrame::RTN);

    std::ofstream nominalFile("notebook/manoeuvre/nominal.csv");
    std::ofstream manoeuvreFile("notebook/manoeuvre/manoeuvre.csv");

    nominalFile << "time,x,y,z,u,v,w\n";
    manoeuvreFile << "time,x,y,z,u,v,w\n";

    nominalFile << std::setprecision(16);
    manoeuvreFile << std::setprecision(16);

    auto writeState = [](std::ofstream& file, double time, const StateVector& state){
        const auto& r = state.getPosition();
        const auto& v = state.getVelocity();
        file << time << ","
             << r.x() << "," << r.y() << "," << r.z() << ","
             << v.x() << "," << v.y() << "," << v.z()
             << "\n";
    };

    StateVector nominalState = initialState;
    StateVector manoeuvreState = initialState;

    constexpr int steps{static_cast<int>(duration / dt)};
    constexpr int burnStep{static_cast<int>(burnTime / dt)};

    for (int k = 0; k <= steps; ++k) {
        const double t = k * dt;
        
        // Apply impulsive manoeuvre at t = 1800 s
        if (k == burnStep) {
            // Save state immediately before the burn
            writeState(manoeuvreFile,t, manoeuvreState);
            // Apply instantaneous velocity change
            manoeuvreState = manoeuvre.apply(manoeuvreState);
        }

        // Save nominal state
        writeState(nominalFile, t, nominalState);
        
        // Save post-burn state at the same epoch
        writeState(manoeuvreFile, t, manoeuvreState);
        
        if (k == steps)
            break;
       
        // Propagate both trajectories
        const std::vector<StateVector> nominalTrajectory = propagator.propagate(nominalState, dt);
        const std::vector<StateVector> manoeuvreTrajectory = propagator.propagate(manoeuvreState, dt);
        
        nominalState = nominalTrajectory.back();
        manoeuvreState = manoeuvreTrajectory.back();

    }

    std::cout << "Propagation completed successfully.\n";

    return 0;

}
