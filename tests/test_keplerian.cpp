#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "State/KeplerianElements.h"
#include "Common/Constants.h"


TEST_CASE("Circular equatorial orbit", "[Keplerian]")
{
    const double a = 7000e3;

    KeplerianElements orbit(
        a,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    );

    const StateVector state = orbit.toCartesian();

    const Eigen::Vector3d position = state.getPosition();
    const Eigen::Vector3d velocity = state.getVelocity();

    const double expectedVelocity =
        std::sqrt(EarthConstants::MU / a);

    REQUIRE(std::abs(position.x() - a) < 1e-6);
    REQUIRE(std::abs(position.y()) < 1e-6);
    REQUIRE(std::abs(position.z()) < 1e-6);

    REQUIRE(std::abs(velocity.x()) < 1e-9);
    REQUIRE(std::abs(velocity.y() - expectedVelocity) < 1e-9);
    REQUIRE(std::abs(velocity.z()) < 1e-9);
}

TEST_CASE("Inclined circular orbit", "[Keplerian]")
{
    constexpr double degToRad = MathsConstants::pi / 180.0;

    const double a = 7000e3;
    const double inclination = 60.0 * degToRad;

    KeplerianElements orbit(
        a,
        0.0,
        inclination,
        0.0,
        0.0,
        0.0
    );

    const StateVector state = orbit.toCartesian();

    const Eigen::Vector3d velocity = state.getVelocity();

    const double v = std::sqrt(EarthConstants::MU / a);

    REQUIRE(std::abs(velocity.x()) < 1e-9);

    REQUIRE(
        std::abs(velocity.y() - v * std::cos(inclination))
        < 1e-9
    );

    REQUIRE(
        std::abs(velocity.z() - v * std::sin(inclination))
        < 1e-9
    );
}

TEST_CASE("Keplerian orbital invariants", "[Keplerian]")
{
    const double a = 7000e3;
    const double e = 0.1;

    KeplerianElements orbit(
        a,
        e,
        0.5,
        0.3,
        0.2,
        1.0
    );

    const StateVector state = orbit.toCartesian();

    const Eigen::Vector3d r = state.getPosition();
    const Eigen::Vector3d v = state.getVelocity();

    // Specific orbital energy
    const double energy =
        0.5 * v.squaredNorm()
        - EarthConstants::MU / r.norm();

    const double expectedEnergy =
        -EarthConstants::MU / (2.0 * a);

    // Specific angular momentum
    const double h = r.cross(v).norm();

    const double expectedH =
        std::sqrt(
            EarthConstants::MU * a * (1.0 - e * e)
        );

    REQUIRE(
        std::abs((energy - expectedEnergy) / expectedEnergy)
        < 1e-12
    );

    REQUIRE(
        std::abs((h - expectedH) / expectedH)
        < 1e-12
    );
}