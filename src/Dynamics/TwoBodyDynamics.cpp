#include "Dynamics/TwoBodyDynamics.h"
#include "Eigen/Core"
#include "State/State.h"


TwoBodyDynamics::TwoBodyDynamics(double mu): mMu(mu) {};

StateVector TwoBodyDynamics::derivative(const StateVector& state) const {

    const Eigen::Vector3d& r = state.getPosition();
    const Eigen::Vector3d& v = state.getVelocity();

    const double rMagnitude = r.norm();

    Eigen::Vector3d acceleration = -(mMu * r) / (rMagnitude*rMagnitude*rMagnitude); // a = - mu/r^3 *r

    return StateVector(v, acceleration);
}

