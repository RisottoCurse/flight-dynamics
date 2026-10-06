#include "Dynamics/J2Perturbation.h"
#include "Eigen/Core"

J2Perturbation::J2Perturbation(double mu, double earthRadius, double j2):
    mMu(mu),
    mEarthRadius(earthRadius),
    mJ2Perturbation(j2)
{}

auto J2Perturbation::acceleration(const Eigen::Vector3d& position) const -> Eigen::Vector3d {

    const double x = position.x();
    const double y = position.y();
    const double z = position.z();

    const double r2 = position.squaredNorm();
    const double r = std::sqrt(r2);
    const double r5 = r*r*r*r*r;
    const double z2 = z * z;
    
    const double constants = (3.0 * mJ2Perturbation * mMu * (mEarthRadius * mEarthRadius)) / (2.0 * r5);

    const double xyTerm = ((5.0 * z2) / r2) - 1.0;
    const double zTerm = ((5.0 * z2) / r2) - 3.0;

    Eigen::Vector3d acceleration;

    acceleration.x() = constants * x * xyTerm;
    acceleration.y() = constants * y * xyTerm;
    acceleration.z() = constants * z * zTerm;

    return acceleration;

}