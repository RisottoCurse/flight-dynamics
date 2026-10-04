#pragma once 

#include "State/State.h"


class TwoBodyDynamics {

    double mMu;

public:

    explicit TwoBodyDynamics(double mu);

    StateVector derivative(const StateVector& state) const;

};