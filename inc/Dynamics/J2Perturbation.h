#pragma once

#include <Eigen/Dense>


class J2Perturbation {

    double mMu;
    double mEarthRadius;
    double mJ2Perturbation;

public: 
    J2Perturbation(double mu, double earthRadius, double j2);
    auto acceleration(const Eigen::Vector3d& position) const -> Eigen::Vector3d;
};
