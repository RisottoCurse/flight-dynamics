#include "Manoeuvre/ImpulsiveManoeuvre.h"
#include "Common/maths.h"

ImpulsiveManoeuvre::ImpulsiveManoeuvre(const Vector3& deltaV, ManoeuvreFrame frame): 
    mDeltaV(deltaV), mFrame(frame) 
    {}

auto ImpulsiveManoeuvre::deltaVECI(const StateVector& state) const -> Vector3 {

    if (mFrame == ManoeuvreFrame::ECI) {
        return mDeltaV;
    }

    else if (mFrame == ManoeuvreFrame::RTN) {
        
        //  Convert RTN to ECI
        const Vector3 position = state.getPosition();
        const Velocity velocity = state.getVelocity();

        // Calculate the angular momentum vector
        const Vector3 h = position.cross(velocity);

        if (!position.allFinite() || !velocity.allFinite() || !mDeltaV.allFinite() || position.norm() == 0 || h.norm() == 0) {
            throw std::runtime_error("Cannot construct RTN frame due to invalid position, velocity, or deltaV vector.");
        }
        
        // Calculate the unit vectors of the RTN frame
        const Vector3 rHat = position.normalized();
        const Vector3 nHat = h.normalized();
        const Vector3 tHat = nHat.cross(rHat);

        // Convert RTN deltaV into ECI

        const Vector3 deltaV = mDeltaV.x() * rHat + mDeltaV.y() * tHat + mDeltaV.z() * nHat;

        return deltaV;
    }
    
    else {
        throw std::runtime_error("Invalid manoeuvre frame specified.");
    }

}


auto ImpulsiveManoeuvre::apply(const StateVector& state) const -> StateVector {

    const Vector3 position = state.getPosition();
    const Velocity velocity = state.getVelocity();

    // convert manoeuvre to ECI
    const Vector3 delataV = deltaVECI(state);

    // Position remains unchanged
    const Vector3 positionUpdated = position;

    // Apply instantaneous change in velocity
    const Velocity velocityUpdated = velocity + delataV;

    return StateVector(positionUpdated, velocityUpdated);
}

auto ImpulsiveManoeuvre::magnitude() const -> double {
    return mDeltaV.norm();
}