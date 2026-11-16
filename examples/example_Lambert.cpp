#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "Common/Constants.h"
#include "Dynamics/OrbitalDynamics.h"
#include "IO/TrajectoryWriter.h"
#include "Lambert/Lambert.h"
#include "Propagator/Propagator.h"
#include "State/State.h"

int main() {
    constexpr double mu{EarthConstants::MU};
    constexpr double dt{1.0};

    // All solver inputs use SI units: metres, seconds, and m^3/s^2.
    const Cartesian position1(6045e3, 3490e3, 0.0);
    const Cartesian position2(12214.839e3, 10249.467e3, 2000e3);

    const double deltaT = 1800;

    Lambert lambert(mu);
    const auto result = lambert.lambertSolver(position1, position2, deltaT);
    if (!result) {
        std::cerr << "Lambert solver failed with status code: " << result.error() << '\n';
        return 1;
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n===== LAMBERT TRANSFER =====\n";
    std::cout << "Initial position [m]: " << position1.transpose() << '\n';
    std::cout << "Target position [m]: " << position2.transpose() << '\n';
    std::cout << "Time of flight [s]: " << deltaT << '\n';
    std::cout << "Departure velocity [m/s]: " << result->initialVelocity.transpose() << '\n';
    std::cout << "Arrival velocity [m/s]: " << result->finalVelocity.transpose() << '\n';
    std::cout << "Transfer semi-major axis [km]: " << result->semiMajorAxis / 1000.0 << '\n';

    // Lambert assumes two-body motion, so disable J2 for the numerical check.
    OrbitalDynamics dynamics(mu);
    dynamics.enableJ2(false);
    Propagator propagator(dynamics, dt);
    const StateVector initialState(position1, result->initialVelocity);
    const auto trajectory = propagator.propagate(initialState, deltaT);
    const auto& finalState = trajectory.back();

 
    TrajectoryWriter writer("notebook/Lambert/lambert_transfer.csv");
    double time = 0.0;
    for (std::size_t i = 0; i < trajectory.size(); ++i) {
        writer.write(time, trajectory[i], 0);
        time += std::min(dt, deltaT - time);
    }
    writer.write(deltaT, StateVector(position2, result->finalVelocity), 1);
    std::cout << "Trajectory saved to notebook/Lambert/lambert_transfer.csv\n";

    std::cout << std::scientific;
    std::cout << "Arrival position error [m]: "
              << (finalState.getPosition() - position2).norm() << '\n';
    std::cout << "Arrival velocity error [m/s]: "
              << (finalState.getVelocity() - result->finalVelocity).norm() << '\n';

    return 0;
}
