#include "Propagator/Propagator.h"
#include "State/State.h"
#include <algorithm>

Propagator::Propagator(const TwoBodyDynamics& dynamics, double stepSize):
    mDynamics(dynamics),
    mStepSize(stepSize) {}


auto Propagator::rk4Step(const StateVector& state, double dt) const -> StateVector {

    const StateVector k1 = mDynamics.derivative(state);
    
    const StateVector k2 = mDynamics.derivative(
        StateVector(
            state.getPosition() + 0.5 * dt * k1.getPosition(),
            state.getVelocity() + 0.5 * dt * k1.getVelocity()
        )
    );

    const StateVector k3 = mDynamics.derivative(
        StateVector(
            state.getPosition()  + 0.5 * dt * k2.getPosition() ,
            state.getVelocity() + 0.5 * dt * k2.getVelocity()
        )
    );

    const StateVector k4 = mDynamics.derivative(
        StateVector(
            state.getPosition()  + dt * k3.getPosition() ,
            state.getVelocity() + dt * k3.getVelocity()
        )
    );

    Eigen::Vector3d position = state.getPosition() + (dt / 6) *
        (k1.getPosition()
        + 2.0 * k2.getPosition()
        + 2.0 * k3.getPosition()
        + k4.getPosition());

    Eigen::Vector3d velocity = state.getVelocity() + (dt / 6) *
        (k1.getVelocity()
        + 2.0 * k2.getVelocity()
        + 2.0 * k3.getVelocity()
        + k4.getVelocity());

    return StateVector(position, velocity);
};

auto Propagator::propagate(const StateVector& initialState, double duration) const -> std::vector<StateVector> {

    std::vector<StateVector> trajectory;
    StateVector state = initialState;
    double elapsedTime{0.0};
    trajectory.push_back(state);

    while (elapsedTime < duration) {
        const double dt = std::min(mStepSize,duration - elapsedTime);
        state = rk4Step(state, dt);
        elapsedTime += dt; 
        trajectory.push_back(state);

    }

    return trajectory;
};