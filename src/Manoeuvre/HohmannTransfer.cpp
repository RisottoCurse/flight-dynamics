#include <cmath>

#include "Manoeuvre/HohmannTransfer.h"
#include "Common/Constants.h"



HohmannTransfer::HohmannTransfer(double mu): 
    mMu(mu) 
    {}



auto HohmannTransfer::hohmannTransfer(const double r1, const double r2) -> HohmannTransferResult {

    // Step 1: Calculate the transfer orbit
    const double transSemiMajorAxis = (r1 + r2) * 0.5;
    const double eccentricity = (r2 - r1) / (r2 + r1);

    //  Step 2: Calculate the first impulse
    const double v1 = std::sqrt(mMu/r1);
    const double transV1 = std::sqrt(((2*mMu)/r1) - (mMu/transSemiMajorAxis));
    const double deltav1 = transV1 - v1;

    // Step 3: Propagate along transfer orbit
    const double orbitalPeriod = MathsConstants::pi * std::sqrt((transSemiMajorAxis * transSemiMajorAxis * transSemiMajorAxis)/mMu);

    // Step 4: Calculate the second impulse
    const double v2 = std::sqrt(mMu/r2);
    const double transV2 = std::sqrt(((2*mMu)/r2) - (mMu/transSemiMajorAxis));
    const double deltav2 = v2 - transV2;

    return {
            .deltaV1 = deltav1,
            .deltaV2 = deltav2,
            .semiMajorAxis = transSemiMajorAxis,
            .eccentricity = eccentricity,
            .semiOrbitalPeriod = orbitalPeriod
    };


}

