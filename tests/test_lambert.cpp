#include "Lambert/Lambert.h"
#include "Dynamics/OrbitalDynamics.h"
#include "Propagator/Propagator.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <limits>
#include <numbers>

TEST_CASE("Lambert reproduces a circular quarter orbit in either orientation", "[Lambert]") {
    constexpr double mu = 3.986004418e14;
    constexpr double radius = 7.0e6;
    const double speed = std::sqrt(mu / radius);
    const double duration = 0.5 * std::numbers::pi * radius / speed;
    for (double direction : {-1.0, 1.0}) {
        CAPTURE(direction);
        const auto result = Lambert(mu).lambertSolver(
            Cartesian(radius, 0, 0), Cartesian(0, direction * radius, 0), duration);
        REQUIRE(result.has_value());
        CHECK(result->initialVelocity.isApprox(Velocity(0, direction * speed, 0), 1e-10));
        CHECK(result->finalVelocity.isApprox(Velocity(-speed, 0, 0), 1e-10));
        CHECK(result->semiMajorAxis == Catch::Approx(radius).epsilon(1e-10));
    }
}

TEST_CASE("Lambert solutions reach both endpoint position and velocity", "[Lambert]") {
    // Non-coplanar with the XY plane, with unequal endpoint radii.
    const Cartesian r1(5.0e6, 1.0e7, 2.1e6);
    const Cartesian r2(-1.46e7, 2.5e6, 7.0e6);
    constexpr double mu = 3.986004418e14;
    const double s = 0.5 * (r1.norm() + r2.norm() + (r2 - r1).norm());
    const double beta = 2.0 * std::asin(std::sqrt((s - (r2 - r1).norm()) / s));
    const double minEnergyTime = std::sqrt(std::pow(s * 0.5, 3) / mu) *
        (std::numbers::pi - beta + std::sin(beta));
    // Check the lower branch, the minimum-energy endpoint, and upper branch.
    for (double duration : {3600.0, minEnergyTime, 2.0 * minEnergyTime}) {
        DYNAMIC_SECTION("Time of flight: " << duration) {
            const auto result = Lambert(mu).lambertSolver(r1, r2, duration);
            REQUIRE(result.has_value());
            OrbitalDynamics dynamics(mu);
            dynamics.enableJ2(false);
            const auto endpoint = Propagator(dynamics, 1.0).propagate(
                StateVector(r1, result->initialVelocity), duration).back();
            CHECK((endpoint.getPosition() - r2).norm() < 0.1);
            CHECK((endpoint.getVelocity() - result->finalVelocity).norm() < 1e-4);
            const double energy = 0.5 * result->initialVelocity.squaredNorm() - mu / r1.norm();
            CHECK(-mu / (2.0 * energy) == Catch::Approx(result->semiMajorAxis).epsilon(1e-10));
            CHECK(r1.cross(result->initialVelocity).isApprox(
                r2.cross(result->finalVelocity), 1e-10));
        }
    }
}

TEST_CASE("Lambert rejects invalid inputs", "[Lambert]") {
    const Cartesian r1(1, 0, 0);
    const Cartesian r2(0, 1, 0);
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    for (double mu : {0.0, -1.0, nan, inf}) {
        CAPTURE(mu);
        const auto result = Lambert(mu).lambertSolver(r1, r2, 1.0);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == StatusCode::INVALID_INPUT);
    }
    for (double duration : {0.0, -1.0, nan, inf}) {
        CAPTURE(duration);
        const auto result = Lambert(1).lambertSolver(r1, r2, duration);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == StatusCode::INVALID_INPUT);
    }
    for (const Cartesian& position : {Cartesian::Zero().eval(), Cartesian(nan, 0, 0), Cartesian(inf, 0, 0)}) {
        for (bool invalidDeparture : {true, false}) {
            DYNAMIC_SECTION("Invalid " << (invalidDeparture ? "departure: " : "arrival: ")
                            << position.transpose()) {
                const auto result = invalidDeparture
                    ? Lambert(1).lambertSolver(position, r2, 1.0)
                    : Lambert(1).lambertSolver(r1, position, 1.0);
                REQUIRE_FALSE(result.has_value());
                CHECK(result.error() == StatusCode::INVALID_INPUT);
            }
        }
    }
}

TEST_CASE("Lambert rejects ambiguous geometry and non-elliptic flight times", "[Lambert]") {
    const Cartesian r1(1, 0, 0);
    for (const Cartesian& r2 : {r1, Cartesian(2, 0, 0), Cartesian(-1, 0, 0)}) {
        DYNAMIC_SECTION("Collinear endpoint: " << r2.transpose()) {
            const auto result = Lambert(1).lambertSolver(r1, r2, 1.0);
            REQUIRE_FALSE(result.has_value());
        }
    }
    const double s = 1.0 + std::sqrt(2.0) * 0.5;
    const double q = s - std::sqrt(2.0);
    const double parabolicTime = std::sqrt(2.0) / 3.0 * (s * std::sqrt(s) - q * std::sqrt(q));
    for (double duration : {parabolicTime, 0.5 * parabolicTime}) {
        DYNAMIC_SECTION("Non-elliptic time of flight: " << duration) {
            const auto result = Lambert(1).lambertSolver(r1, Cartesian(0, 1, 0), duration);
            REQUIRE_FALSE(result.has_value());
            // The public solver currently maps bisection failures to this status.
            CHECK(result.error() == StatusCode::A_MAX_LESS_THAN_MIN);
        }
    }
    SECTION("Near-parabolic elliptic transfer converges") {
        const auto result = Lambert(1).lambertSolver(r1, Cartesian(0, 1, 0), parabolicTime * 1.0001);
        REQUIRE(result.has_value());
        CHECK(result->initialVelocity.allFinite());
    }
}

TEST_CASE("Lambert recovers an eccentric orbit from periapsis to quadrature", "[Lambert]") {
    // Independent analytic reference in normalized units: mu = a = 1,
    // eccentricity = 0.4, true anomalies 0 and pi/2.
    constexpr double eccentricity = 0.4;
    constexpr double p = 1.0 - eccentricity * eccentricity;
    const double eccentricAnomaly = std::acos(eccentricity);
    const double duration = eccentricAnomaly - eccentricity * std::sin(eccentricAnomaly);
    const auto result = Lambert(1.0).lambertSolver(
        Cartesian(1.0 - eccentricity, 0, 0), Cartesian(0, p, 0), duration);

    REQUIRE(result.has_value());
    CHECK(result->semiMajorAxis == Catch::Approx(1.0).epsilon(1e-10));
    CHECK(result->initialVelocity.isApprox(
        Velocity(0, (1.0 + eccentricity) / std::sqrt(p), 0), 1e-10));
    CHECK(result->finalVelocity.isApprox(
        Velocity(-1.0 / std::sqrt(p), eccentricity / std::sqrt(p), 0), 1e-10));
}

TEST_CASE("Lambert circular solutions are independent of orbital plane and unit scale", "[Lambert]") {
    for (double radius : {1.0, 7.0e6}) {
        DYNAMIC_SECTION("Radius: " << radius) {
            // Keep mu/r^3 = 1 so every quarter orbit takes pi/2 seconds.
            const double mu = radius * radius * radius;
            const Cartesian departure(0, 0, radius);
            const Cartesian arrival(radius, 0, 0);
            const auto result = Lambert(mu).lambertSolver(
                departure, arrival, std::numbers::pi / 2.0);

            REQUIRE(result.has_value());
            CHECK(result->semiMajorAxis == Catch::Approx(radius).epsilon(1e-10));
            CHECK(result->initialVelocity.isApprox(Velocity(radius, 0, 0), 1e-10));
            CHECK(result->finalVelocity.isApprox(Velocity(0, 0, -radius), 1e-10));
        }
    }
}
