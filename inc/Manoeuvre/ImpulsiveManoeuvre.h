/**
* @file impulsiveManoeuvre.h
* @author Ashutosh Madhu
* @brief This file contains the definition of the ImpulsiveManoeuvre class, which represents an impulsive manoeuvre in space.
* @date 2026-10-12
*/
#pragma once

#include "State/State.h"
#include "Common/maths.h"

enum class ManoeuvreFrame {
    ECI,
    RTN
};

class ImpulsiveManoeuvre {
    
    Vector3 mDeltaV;
    ManoeuvreFrame mFrame;

public: 

    explicit ImpulsiveManoeuvre(const Vector3& deltaV, ManoeuvreFrame frame = ManoeuvreFrame::ECI);

    // apply the manoeuvre to a satellite state
    [[nodiscard]] auto apply(const StateVector& state) const -> StateVector;
    
    // convert manoeuvre into ECI coordinates
    [[nodiscard]] auto deltaVECI(const StateVector& state) const -> Vector3;
    
    // calculate total delta V magnitude
    [[nodiscard]] auto magnitude() const -> double;
};