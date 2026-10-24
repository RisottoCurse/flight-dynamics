#include <iostream>
#include <iomanip>
#include <fstream>

#include "State/State.h"
#include "Common/maths.h"
#include "Common/Constants.h"
#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"
#include "Manoeuvre/HohmannTransfer.h"


int main() {

    // Simulation Parameters
    constexpr double initialRadius{7000e3}; // 7000km
    constexpr double finalRadius{12000e3}; // 12000 km

    constexpr double mu{EarthConstants::MU};
    constexpr double dt{1};

    // Calculate Hohmann Transfer 
    HohmannTransfer hohmann(mu);
    const auto hohmannTransferResult = hohmann.hohmannTransfer(initialRadius, finalRadius);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\n===== HOHMANN TRANSFER =====\n";
    std::cout << "Initial radius: "
              << initialRadius / 1000.0 << " km\n";
    std::cout << "Final radius: "
              << finalRadius / 1000.0 << " km\n";
    std::cout << "Delta V1: "
              << hohmannTransferResult.deltaV1 << " m/s\n";
    std::cout << "Delta V2: "
              << hohmannTransferResult.deltaV2 << " m/s\n";
    std::cout << "Transfer time: "
              << hohmannTransferResult.semiOrbitalPeriod << " s\n";

    
    // Define StateVector for initial orbit
    const double v1 = std::sqrt(mu/initialRadius);

    StateVector state(
        Vector3(initialRadius, 0.0, 0.0),
        Velocity(0.0, v1, 0.0)
    );

    // Dynamics Config
    OrbitalDynamics dynamics(mu);
    Propagator propagate(dynamics, dt);


    // Txt Output
    std::ofstream file("notebook/HohmannTransfer/hohmann_transfer.csv");
    
    if (!file.is_open()) {
        std::cerr << "Error opening CSV file\n";
        return 1;
    }

    file << "time,x,y,z,u,v,w,phase\n";

    auto saveState = [&](double t, const StateVector& s, int phase) {

        const auto& r = s.getPosition();
        const auto& v = s.getVelocity();

        file << std::setprecision(16)
             << t << ","
             << r.x() << ","
             << r.y() << ","
             << r.z() << ","
             << v.x() << ","
             << v.y() << ","
             << v.z() << ","
             << phase << "\n";
    };

    double time{0.0};
    saveState(time, state, 0);

    // First Impulse 
    const Vector3 deltaV1 = hohmannTransferResult.deltaV1 * state.getVelocity().normalized(); // unit vector 
    state = StateVector(state.getPosition(), state.getVelocity() + deltaV1);

    saveState(time, state, 1);

    // Propagate the orbit
    while(time < hohmannTransferResult.semiOrbitalPeriod) {

        const double duration = std::min(propagate.getStepSize(), hohmannTransferResult.semiOrbitalPeriod - time);
        state = propagate.singlePropagate(state, duration);
        time += duration;
        saveState(time, state, 1);

    }

    // Second Impulse
    const Vector3 deltaV2 = hohmannTransferResult.deltaV2 * state.getVelocity().normalized();
    state = StateVector(state.getPosition(), state.getVelocity() + deltaV2);
    
    saveState(time, state, 1);

    // Propagate final orbit

    const double finalPeriod = 2.0 * MathsConstants::pi * std::sqrt((finalRadius*finalRadius*finalRadius)/mu);
    const double endTime = time + finalPeriod;

    while (time < endTime) {
        const double duration = std::min(dt, endTime - time);
        Propagator stepPropagator(dynamics, duration);
        state = stepPropagator.singlePropagate(state, duration);
        time += duration;
        saveState(time, state, 2);
    }

    file.close();
    std::cout << "\nTrajectory saved to hohmann_transfer.csv\n";
    return 0;
}