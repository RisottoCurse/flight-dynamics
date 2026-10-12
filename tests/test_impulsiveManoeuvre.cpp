#include <catch2/catch_test_macros.hpp>

#include <Eigen/Core>

#include "State/State.h"
#include "Manoeuvre/ImpulsiveManoeuvre.h"

TEST_CASE(
    "Impulsive manoeuvre preserves position",
    "[manoeuvre]"
)
{
    const StateVector state(
        Vector3(7000e3, 0.0, 0.0),
        Velocity(0.0, 7500.0, 0.0)
    );

    const ImpulsiveManoeuvre manoeuvre(
        Vector3(0.0, 100.0, 0.0),
        ManoeuvreFrame::ECI
    );

    const StateVector updated =
        manoeuvre.apply(state);

    REQUIRE(
        updated.getPosition().isApprox(
            state.getPosition(),
            1e-12
        )
    );
}


TEST_CASE(
    "Impulsive manoeuvre updates velocity",
    "[manoeuvre]"
)
{
    const StateVector state(
        Vector3(7000e3, 0.0, 0.0),
        Velocity(0.0, 7500.0, 0.0)
    );

    const ImpulsiveManoeuvre manoeuvre(
        Vector3(0.0, 100.0, 0.0),
        ManoeuvreFrame::ECI
    );

    const StateVector updated =
        manoeuvre.apply(state);

    const Velocity expectedVelocity(
        0.0,
        7600.0,
        0.0
    );

    REQUIRE(
        updated.getVelocity().isApprox(
            expectedVelocity,
            1e-12
        )
    );
}


TEST_CASE(
    "RTN manoeuvre converts correctly to ECI",
    "[manoeuvre]"
)
{
    const StateVector state(
        Vector3(7000e3, 0.0, 0.0),
        Velocity(0.0, 7500.0, 0.0)
    );

    const ImpulsiveManoeuvre manoeuvre(
        Vector3(0.0, 100.0, 0.0),
        ManoeuvreFrame::RTN
    );

    const Vector3 deltaV =
        manoeuvre.deltaVECI(state);

    const Vector3 expected(
        0.0,
        100.0,
        0.0
    );

    REQUIRE(
        deltaV.isApprox(expected, 1e-12)
    );
}