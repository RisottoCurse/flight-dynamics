#include <iostream>
#include <iomanip>

#include "State/State.h"
#include "Common/maths.h"
#include "Common/Constants.h"
#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"
#include "Manoeuvre/HohmannTransfer.h"
#include "IO/TrajectoryWriter.h"
#include "Manoeuvre/ImpulsiveManoeuvre.h"

enum class TrajectoryPhase {
    Initial = 0,
    Transfer = 1,
    Final = 2
};

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
    TrajectoryWriter writer("notebook/HohmannTransfer/hohmann_transfer.csv");
    writer.write(0.0, state, static_cast<int>(TrajectoryPhase::Initial));

    // First Impulse 
    ImpulsiveManoeuvre firstImpulse(Vector3(0.0, hohmannTransferResult.deltaV1, 0.0 ), ManoeuvreFrame::RTN);
    state = firstImpulse.apply(state);
    writer.write(0.0, state, static_cast<int>(TrajectoryPhase::Transfer));

    // Propagate the orbit
    double time{0.0};
    while(time < hohmannTransferResult.semiOrbitalPeriod) {

        const double duration = std::min(propagate.getStepSize(), hohmannTransferResult.semiOrbitalPeriod - time);
        state = propagate.singlePropagate(state, duration);
        time += duration;
        writer.write(time, state, static_cast<int>(TrajectoryPhase::Transfer));

    }

    // Second Impulse
    ImpulsiveManoeuvre secondImpulse(Vector3(0.0, hohmannTransferResult.deltaV2, 0.0), ManoeuvreFrame::RTN);
    state = secondImpulse.apply(state);
    writer.write(time, state, static_cast<int>(TrajectoryPhase::Final));

    // Propagate final orbit
    const double finalPeriod = 2.0 * MathsConstants::pi * std::sqrt((finalRadius*finalRadius*finalRadius)/mu);
    const double endTime = time + finalPeriod;

    while (time < endTime) {
        const double duration = std::min(dt, endTime - time);
        state = propagate.singlePropagate(state, duration);
        time += duration;
        writer.write(time, state, static_cast<int>(TrajectoryPhase::Final));
    }

    std::cout << "\nTrajectory saved to hohmann_transfer.csv\n";
    return 0;
}