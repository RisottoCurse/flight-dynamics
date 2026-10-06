cmake_minimum_required(VERSION 3.20)

project(MyProject
    VERSION 1.0
    DESCRIPTION "Flight Dynamics Project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Eigen
# --------------------------------------------------

add_subdirectory(external/eigen)


# --------------------------------------------------
# Flight Dynamics library
# --------------------------------------------------

add_library(flight_dynamics
    src/Dynamics/OrbitalDynamics.cpp
    src/Dynamics/J2Perturbation.cpp
    src/Propagator/Propagator.cpp
)

target_include_directories(flight_dynamics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(flight_dynamics
    PUBLIC
        Eigen3::Eigen
)


# --------------------------------------------------
# Main executable
# --------------------------------------------------

add_executable(main
    src/main.cpp
)

target_link_libraries(main
    PRIVATE
        flight_dynamics
)


# --------------------------------------------------
# Catch2
# --------------------------------------------------

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH
    ${catch2_SOURCE_DIR}/extras
)


# --------------------------------------------------
# Tests
# --------------------------------------------------

enable_testing()

add_subdirectory(tests)

# --------------------------------------------------
# Examples
# --------------------------------------------------

add_subdirectory(examples)
