/**
 * @file Lambert.h
 * @author Ashutosh Madhu
 * @brief Initial Lambert Solver which which calculate the required delta v for two given postition vectors and time of flights.
 * @version 0.1
 * @date 2026-11-06
 * 
 * 
 */

 #pragma once

 #include "Common/maths.h"
 #include "Common/StatusCodes.h"

 #include <expected>

 enum class transferDirection {
    Prograde,
    Retrograde
 };

 enum class LambertBranch {
    Upper,
    Lower
 };

 struct LambertResult {
    Velocity    initialVelocity;
    Velocity    finalVelocity;

    double  semiMajorAxis;
 };

class Lambert{
    double  mMu;
public: 
    explicit Lambert(double mu);

    auto lambertSolver(const Cartesian position1, const Cartesian position2, const double deltaT) const -> std::expected<LambertResult, StatusCode>; 

private:
    auto bisectionSolver(const double deltaT, const double semiPerimeter, const double chord) const -> std::expected<double, StatusCode>;
 };