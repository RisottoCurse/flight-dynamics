#include "Lambert/Lambert.h"

Lambert::Lambert(double mu) :mMu(mu) {}

auto Lambert::lambertSolver(const Cartesian position1, const Cartesian position2, const double deltaT) const -> std::expected<LambertResult, StatusCode> {

    // 1. Validate inputs

    if (!position1.allFinite() || !position2.allFinite() ||
        !std::isfinite(deltaT) ||
        !std::isfinite(mMu) ||
        mMu <= 0.0 ||
        deltaT <= 0.0) {
        return std::unexpected(StatusCode::INVALID_INPUT);
    }
    
    // Numerically Propagate s/c 2 if v2 is provided
    
    // Calculate chord length
    const double chord = (position1 - position2).norm();

    // Calculate semi-perimeter
    const double semiPerimeter = (position1.norm() + position2.norm() + chord) * 0.5;

    // Find semi-major axis of transfer ellipse using bisection
    const auto result = bisectionSolver(deltaT, semiPerimeter, chord);
    if (!result) {return std::unexpected(StatusCode::A_MAX_LESS_THAN_MIN);}
    const double transferSemiMajorAxis = std::move(*result);

    // Define unit vectors for velocity
    const Vector3 pos1Hat = position1 / position1.norm();
    const Vector3 pos2Hat = position2 / position2.norm();
    const Vector3 chordHat = (position2 - position1) / chord;

    // auxilary angles
    const double alpha = 2.0 * std::asin(std::sqrt(semiPerimeter / (2.0 * transferSemiMajorAxis)));
    const double beta = 2.0 * std::asin(std::sqrt((semiPerimeter - chord) / (2.0 * transferSemiMajorAxis)));

    const double A = std::sqrt(mMu / (4.0 * transferSemiMajorAxis));
    const double B = 1.0 / std::tan(alpha / 2.0);
    const double C = 1.0 / std::tan(beta / 2.0);

    const Velocity v1 = A * ((B + C) * chordHat + (C - B) * pos1Hat);
    const Velocity v2 = A * ((B + C) * chordHat - (C - B) * pos2Hat);

    return LambertResult { 
        .initialVelocity = v1,
        .finalVelocity   = v2,
        .semiMajorAxis   = transferSemiMajorAxis

    };

};

auto Lambert::bisectionSolver(const double deltaT, const double semiPerimeter, const double chord) const -> std::expected<double, StatusCode> {

    constexpr int maxIterations = 100;
    constexpr double tolerance = 1e-12;
    
    // 1. Calculate a_min
    const double minSemiMajorAxis = semiPerimeter * 0.5;

    // 2. Compute residual from calculated time of flight minus delta T
    const auto residual = [&](double a){

        const double alpha = 2.0 * std::asin(std::sqrt(semiPerimeter / (2.0 * a)));
        const double beta  = 2.0 * std::asin(std::sqrt((semiPerimeter - chord) / (2.0 * a)));
        const double calculatedTime = std::sqrt((a*a*a)/mMu) * (alpha - beta - (std::sin(alpha) - std::sin(beta)));

        return calculatedTime - deltaT;
    };

    // initial bounds 
    double lowSemiMajorAxis = minSemiMajorAxis;
    double highSemiMajorAxis = 2.0 * minSemiMajorAxis;

    double fLow  = residual(lowSemiMajorAxis);
    double fHigh = residual(highSemiMajorAxis);

    // Expand Upper Bound Until Desired Value is Bracketed
    for (int i = 0; i < maxIterations && fLow * fHigh > 0.0; i++) {
        highSemiMajorAxis *= 2.0;
        fHigh = residual(highSemiMajorAxis);
    }

    if (fLow * fHigh > 0.0) {
        return std::unexpected(StatusCode::NO_CONVERGENCE_IN_BISECTION);
    }

    for (int i = 0; i < maxIterations; i++) {

        const double midSemiMajorAxis = 0.5 * (lowSemiMajorAxis + highSemiMajorAxis);
        const double fMid = residual(midSemiMajorAxis);

        if(std::abs(fMid) < tolerance) {
            return midSemiMajorAxis;
        }

        // Checks whether fLow and fMid have opposite signs hence desired semi major axis lies in between here
        if ((fLow < 0.0 && fMid > 0.0) || (fLow > 0.0 && fMid < 0.0)) {
            highSemiMajorAxis = midSemiMajorAxis;
            fHigh = fMid;
        }

        // Else the desired semi major axis lies in between mid and high
        else {
            lowSemiMajorAxis = midSemiMajorAxis;
            fLow = fMid;
        }
    }

    return std::unexpected(StatusCode::NO_CONVERGENCE_IN_BISECTION);
};
