#pragma once 


#include "Common/maths.h"

class StateVector {

    Cartesian mPosition;
    Velocity mVelocity;

public: 
    StateVector(const Cartesian& position, const Velocity& velocity) : 
    mPosition(position), mVelocity(velocity) 
    {};

    // getters
    [[nodiscard]] auto getPosition() const -> const Cartesian& { return mPosition; }
    [[nodiscard]] auto getVelocity() const -> const Velocity& { return mVelocity; }

};
