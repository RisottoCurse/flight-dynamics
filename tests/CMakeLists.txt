add_executable(unit_tests
    test_state.cpp
    test_dynamics.cpp
    test_propagator.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        flight_dynamics
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

catch_discover_tests(unit_tests)