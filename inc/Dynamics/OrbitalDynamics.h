#pragma once 

#include "Common/Constants.h"
#include "Dynamics/J2Perturbation.h"
#include "State/State.h"


class OrbitalDynamics {

    double mMu;
    bool mJ2Enabled;
    J2Perturbation mJ2;

public:
    explicit OrbitalDynamics(double mu = EarthConstants::MU);
    auto derivative(const StateVector& state) const -> StateVector;
    void enableJ2(bool enabled);

};