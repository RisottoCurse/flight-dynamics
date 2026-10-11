#include "State/KeplerianElements.h"
#include "State/State.h"
#include <cmath>
#include <stdexcept>
#include <Eigen/Core>
#include <Eigen/Geometry>

KeplerianElements::KeplerianElements(

    double semiMajorAxis,
    double eccentricity,
    double inclination,
    double raan,
    double argumentOfPeriapsis,
    double trueAnomaly,
    double mu):
    mSemiMajorAxis(semiMajorAxis),
    mEccentricity(eccentricity),
    mInclination(inclination),
    mRAAN(raan),
    mArgumentOfPeriapsis(argumentOfPeriapsis),
    mTrueAnomaly(trueAnomaly),
    mMu(mu)
{
    if (!std::isfinite(mSemiMajorAxis) ||
        !std::isfinite(mEccentricity) ||
        !std::isfinite(mInclination) ||
        !std::isfinite(mRAAN) ||
        !std::isfinite(mArgumentOfPeriapsis) ||
        !std::isfinite(mTrueAnomaly) ||
        !std::isfinite(mMu) ||
        mSemiMajorAxis <= 0.0 ||
        mEccentricity < 0.0 ||
        mEccentricity >= 1.0 ||
        mMu <= 0.0)
    {
        throw std::invalid_argument("Invalid Keplerian elements");
    }
}

auto KeplerianElements::toCartesian() const -> StateVector {
    
    // Semi-Latus Rectum
    const double p = mSemiMajorAxis * (1 - mEccentricity * mEccentricity);

    // Orbital Radius 
    const double r = p / (1.0 + mEccentricity * std::cos(mTrueAnomaly));

    // Position in Perifocal Frame 
    const Eigen::Vector3d positionPQW(
            r * std::cos(mTrueAnomaly),
            r * std::sin(mTrueAnomaly),
            0.0
        );

    // Velocity in Perifocal Frame
    const double velocityFactor = std::sqrt(mMu / p);

    const Eigen::Vector3d velocityPQW(
        -velocityFactor * std::sin(mTrueAnomaly),
        velocityFactor * (mEccentricity + std::cos(mTrueAnomaly)),
        0.0      
    );

    // Rotation Matrices
     const Eigen::Matrix3d R3_RAAN = Eigen::AngleAxisd(
        mRAAN,
         Eigen::Vector3d::UnitZ()
     ).toRotationMatrix();

     const Eigen::Matrix3d R1_Inclination = Eigen::AngleAxisd(
         mInclination,
          Eigen::Vector3d::UnitX()
     ).toRotationMatrix();

    const Eigen::Matrix3d R3_ArgumentOfPeriapsis = Eigen::AngleAxisd(
        mArgumentOfPeriapsis,
         Eigen::Vector3d::UnitZ()
    ).toRotationMatrix();

    // PQW -> ECI transformation
    const Eigen::Matrix3d rotationMatrix =
        R3_RAAN *
        R1_Inclination *
        R3_ArgumentOfPeriapsis;


    // Transform position and velocity
    const Eigen::Vector3d positionECI = rotationMatrix * positionPQW;
    const Eigen::Vector3d velocityECI = rotationMatrix * velocityPQW;


    return StateVector(positionECI, velocityECI);
}