#pragma once 


#include "Common/maths.h"

class StateVector {

    Vector3 mPosition;
    Velocity mVelocity;

public: 
    StateVector(const Vector3& position, const Velocity& velocity) : 
    mPosition(position), mVelocity(velocity) 
    {};

    // getters
    [[nodiscard]] auto getPosition() const -> const Vector3& { return mPosition; }
    [[nodiscard]] auto getVelocity() const -> const Velocity& { return mVelocity; }

};
