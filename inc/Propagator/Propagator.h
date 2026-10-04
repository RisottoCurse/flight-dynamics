#pragma once

#include "State/State.h"
#include "Dynamics/TwoBodyDynamics.h"

class Propagator {

    StateVector rk4Step(const StateVector& state, double dt) const;

    const TwoBodyDynamics& mDynamics;
    double mStepSize;

public: 

    Propagator(const TwoBodyDynamics& dynamics, double stepSize);
    std::vector<StateVector> propagate(const StateVector& initialState, double duration) const;

    // getters
    [[nodiscard]] auto getStepSize() const -> const double { return mStepSize; }

};