add_executable(example_twoBodyDynamics
    example_twoBodyDynamics.cpp
)

target_link_libraries(example_twoBodyDynamics
    PRIVATE
        flight_dynamics
)

add_executable(example_J2Dynamics
    example_J2Dynamics.cpp
)

target_link_libraries(example_J2Dynamics
    PRIVATE
        flight_dynamics
)

add_executable(example_impulsiveManoeuvre
example_impulsiveManoeuvre.cpp
)

target_link_libraries(example_impulsiveManoeuvre
    PRIVATE
        flight_dynamics
)

add_executable(example_HohmannTransfer
example_HohmannTransfer.cpp
)

target_link_libraries(example_HohmannTransfer
    PRIVATE
        flight_dynamics
)

add_executable(example_Lambert
    example_Lambert.cpp
)

target_link_libraries(example_Lambert
    PRIVATE
        flight_dynamics
)
