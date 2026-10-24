#pragma once

#include "State/State.h"
#include "Dynamics/OrbitalDynamics.h"

class Propagator {

    StateVector rk4Step(const StateVector& state, double dt) const;

    const OrbitalDynamics& mDynamics;
    double mStepSize;

public: 

    Propagator(const OrbitalDynamics& dynamics, double stepSize);
    
    auto propagate(const StateVector& initialState, double duration) const -> std::vector<StateVector>;
    auto singlePropagate(const StateVector& initialState, double dt) const -> StateVector;

    // getters
    [[nodiscard]] auto getStepSize() const -> const double { return mStepSize; }

};