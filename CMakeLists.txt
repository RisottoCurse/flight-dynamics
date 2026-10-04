cmake_minimum_required(VERSION 3.20)

project(MyProject
    VERSION 1.0
    DESCRIPTION "Flight Dynamics Project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/eigen)

# Flight Dynamics library
add_library(flight_dynamics INTERFACE)

target_include_directories(flight_dynamics
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(flight_dynamics
    INTERFACE
        Eigen3::Eigen
)

# Main executable
add_executable(main
    src/main.cpp
    src/Dynamics/TwoBodyDynamics.cpp
    src/Propagator/Propagator.cpp
)

target_link_libraries(main
    PRIVATE
        flight_dynamics
)