#include <cmath>
#include <numbers>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <Eigen/Core>

#include "Common/Constants.h"
#include "Manoeuvre/HohmannTransfer.h"


TEST_CASE("Hohmann transfer semi-major axis", "[Hohmann]")
{
    constexpr double r1 = 7000e3;
    constexpr double r2 = 12000e3;

    HohmannTransfer transfer(EarthConstants::MU);

    const auto result = transfer.hohmannTransfer(r1, r2);

    const double expectedA = (r1 + r2) / 2.0;

    REQUIRE(
        result.semiMajorAxis ==
        Catch::Approx(expectedA).epsilon(1e-12)
    );
}

TEST_CASE("Hohmann transfer eccentricity", "[Hohmann]")
{
    constexpr double r1 = 7000e3;
    constexpr double r2 = 12000e3;

    HohmannTransfer transfer(EarthConstants::MU);

    const auto result = transfer.hohmannTransfer(r1, r2);

    const double expectedE =
        (r2 - r1) / (r2 + r1);

    REQUIRE(
        result.eccentricity ==
        Catch::Approx(expectedE).epsilon(1e-12)
    );
}

TEST_CASE("Hohmann transfer time", "[Hohmann]")
{
    constexpr double mu = EarthConstants::MU;

    constexpr double r1 = 7000e3;
    constexpr double r2 = 12000e3;

    HohmannTransfer transfer(mu);

    const auto result = transfer.hohmannTransfer(r1, r2);

    const double a = (r1 + r2) / 2.0;

    const double expectedTime =
        std::numbers::pi *
        std::sqrt(a * a * a / mu);

    REQUIRE(
        result.semiOrbitalPeriod ==
        Catch::Approx(expectedTime).epsilon(1e-12)
    );
}

TEST_CASE("Hohmann transfer Delta V", "[Hohmann]")
{
    constexpr double mu = EarthConstants::MU;

    constexpr double r1 = 7000e3;
    constexpr double r2 = 12000e3;

    HohmannTransfer transfer(mu);

    const auto result = transfer.hohmannTransfer(r1, r2);

    const double a = (r1 + r2) / 2.0;

    const double vCircular1 =
        std::sqrt(mu / r1);

    const double vTransfer1 =
        std::sqrt(mu * (2.0 / r1 - 1.0 / a));

    const double vCircular2 =
        std::sqrt(mu / r2);

    const double vTransfer2 =
        std::sqrt(mu * (2.0 / r2 - 1.0 / a));

    const double expectedDV1 =
        vTransfer1 - vCircular1;

    const double expectedDV2 =
        vCircular2 - vTransfer2;

    REQUIRE(
        result.deltaV1 ==
        Catch::Approx(expectedDV1).epsilon(1e-12)
    );

    REQUIRE(
        result.deltaV2 ==
        Catch::Approx(expectedDV2).epsilon(1e-12)
    );
}

TEST_CASE("Hohmann transfer orbit reaches target radius", "[Hohmann]")
{
    constexpr double mu = EarthConstants::MU;

    constexpr double r1 = 7000e3;
    constexpr double r2 = 12000e3;

    HohmannTransfer transfer(mu);

    const auto result = transfer.hohmannTransfer(r1, r2);

    const double circularVelocity =
        std::sqrt(mu / r1);

    const double transferVelocity =
        circularVelocity + result.deltaV1;

    const double specificEnergy =
        0.5 * transferVelocity * transferVelocity - mu / r1;

    const double angularMomentum =
        r1 * transferVelocity;

    const double e =
        std::sqrt(
            1.0 +
            2.0 * specificEnergy *
            angularMomentum * angularMomentum /
            (mu * mu)
        );

    const double apoapsis =
        result.semiMajorAxis * (1.0 + e);

    REQUIRE(
        apoapsis ==
        Catch::Approx(r2).epsilon(1e-10)
    );
}