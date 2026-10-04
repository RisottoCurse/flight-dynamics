#pragma once

#include "State/State.h"
#include "Dynamics/OrbitalDynamics.h"

class Propagator {

    StateVector rk4Step(const StateVector& state, double dt) const;

    const OrbitalDynamics& mDynamics;
    double mStepSize;

public: 

    Propagator(const OrbitalDynamics& dynamics, double stepSize);
    std::vector<StateVector> propagate(const StateVector& initialState, double duration) const;

    // getters
    [[nodiscard]] auto getStepSize() const -> const double { return mStepSize; }

};