#include "Dynamics/OrbitalDynamics.h"
#include "Eigen/Core"
#include "State/State.h"


OrbitalDynamics::OrbitalDynamics(double mu): mMu(mu), mJ2Enabled(false), mJ2(mu, EarthConstants::RADIUS, EarthConstants::J2) {};

StateVector OrbitalDynamics::derivative(const StateVector& state) const {

    const Eigen::Vector3d& r = state.getPosition();
    const Eigen::Vector3d& v = state.getVelocity();

    const double rMagnitude = r.norm();

    Eigen::Vector3d acceleration = -(mMu * r) / (rMagnitude*rMagnitude*rMagnitude); // a = - mu/r^3 *r // 2 body dynamics

    if(mJ2Enabled) {
        acceleration += mJ2.acceleration(r); //add acceleration due to J2 to two-body if true
    }

    return StateVector(v, acceleration);
}

void OrbitalDynamics::enableJ2(bool enabled) {
    mJ2Enabled = enabled;
}

