#pragma once

#include "Common/Constants.h"
#include "State/State.h"

class KeplerianElements {

    double mSemiMajorAxis;
    double mEccentricity;
    double mInclination;
    double mRAAN;
    double mArgumentOfPeriapsis;
    double mTrueAnomaly;
    double mMu;

public:

    KeplerianElements(
            double semiMajorAxis,
            double eccentricity,
            double inclination,
            double raan,
            double argumentOfPeriapsis,
            double trueAnomaly,
            double mu = EarthConstants::MU
    );

    // Convert Keplerian elements to Cartsesian State
    [[nodiscard]] auto toCartesian() const -> StateVector;

    // Getters
    [[nodiscard]] auto getSemiMajorAxis() const -> double {return mSemiMajorAxis;}
    [[nodiscard]] auto getEccentricity() const -> double {return mEccentricity;}
    [[nodiscard]] auto getInclination() const -> double {return mInclination;}
    [[nodiscard]] auto getRAAN() const -> double {return mRAAN;}
    [[nodiscard]] auto getArgumentOfPeriapsis() const -> double {return mArgumentOfPeriapsis;}
    [[nodiscard]] auto getTrueAnomaly() const -> double {return mTrueAnomaly;}

};
