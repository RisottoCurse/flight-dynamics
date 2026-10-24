/**
* @file HohmannTransfer.h
* @author Ashutosh Madhu
* @brief This file contains the definition of the Hohmann transfer which is a two-impulse orbital manoeuvre that transfers a spacecraft between two coplanar circular orbits.
* @date 2026-10-21
*/

#pragma once

// for the Hohmann transfer we need to enter the current radius and the desired radius of the transfer
// the output will be the total delta v 

struct HohmannTransferResult{

    double deltaV1;
    double deltaV2;

    double semiMajorAxis;
    double eccentricity;
    double semiOrbitalPeriod;
};


class HohmannTransfer {

    double mRadius1;
    double mRadius2;
    double mMu;

public: 

    explicit HohmannTransfer(double mu);

    auto hohmannTransfer(const double r1, const double r2) -> HohmannTransferResult;


};