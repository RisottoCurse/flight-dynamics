# Flight Dynamics

A C++20 flight-dynamics project for modelling and propagating spacecraft orbits using numerical integration and progressively more realistic force models.

The project is a practical exploration of orbital mechanics, numerical methods, and spacecraft flight dynamics, with an emphasis on clean, modular, testable software.

## Features

- Cartesian spacecraft state representation
- Two-body gravitational dynamics
- Numerical orbit propagation with a fourth-order Runge-Kutta (RK4) integrator
- Eigen-based linear algebra
- Modular force/dynamics model architecture
- Conservation checks for specific orbital energy and specific angular momentum
- MATLAB tools for visualising propagated trajectories
- J2 perturbation modelling (in progress)

## Project Structure

```text
flight-dynamics/
├── inc/
│   ├── Dynamics/
│   │   ├── TwoBodyDynamics.h
│   │   └── J2Dynamics.h
│   ├── Propagator/
│   │   └── Propagator.h
│   └── State/
│       └── StateVector.h
├── src/
│   ├── Dynamics/
│   │   ├── TwoBodyDynamics.cpp
│   │   └── J2Dynamics.cpp
│   ├── Propagator/
│   │   └── Propagator.cpp
│   └── main.cpp
├── test/
├── matlab/
│   ├── plotTrajectory.m
│   ├── angularMomentum.m
│   └── ...
├── external/
│   └── eigen/
├── CMakeLists.txt
├── Makefile
├── .gitignore
└── README.md
```

The exact structure may evolve as additional dynamics models, propagators, and tests are added.

## Mathematical Model

### Two-Body Dynamics

The initial propagator models the spacecraft as a point mass orbiting a central body under Newtonian gravity.

The spacecraft state is

$$
\mathbf{x} =
\begin{bmatrix}
\mathbf{r} \\
\mathbf{v}
\end{bmatrix}
$$

where

$$
\mathbf{r} =
\begin{bmatrix}
x & y & z
\end{bmatrix}^T,
\qquad
\mathbf{v} =
\begin{bmatrix}
v_x & v_y & v_z
\end{bmatrix}^T
$$

are the position and velocity vectors.

The equations of motion are

$$
\dot{\mathbf{r}} = \mathbf{v},
\qquad
\dot{\mathbf{v}} = -\frac{\mu}{r^3}\mathbf{r}
$$

where $\mu$ is the gravitational parameter of the central body and $r = \|\mathbf{r}\|$.

Therefore,

$$
\dot{\mathbf{x}} =
\begin{bmatrix}
\mathbf{v} \\
-\dfrac{\mu}{r^3}\mathbf{r}
\end{bmatrix}.
$$

## Numerical Propagation

The equations of motion are integrated with the classical fourth-order Runge-Kutta method (RK4). For a state $\mathbf{x}_n$ and timestep $h$,

$$
\begin{aligned}
\mathbf{k}_1 &= f(\mathbf{x}_n) \\
\mathbf{k}_2 &= f\left(\mathbf{x}_n + \tfrac{h}{2}\mathbf{k}_1\right) \\
\mathbf{k}_3 &= f\left(\mathbf{x}_n + \tfrac{h}{2}\mathbf{k}_2\right) \\
\mathbf{k}_4 &= f\left(\mathbf{x}_n + h\,\mathbf{k}_3\right)
\end{aligned}
$$

and the propagated state is

$$
\mathbf{x}_{n+1} = \mathbf{x}_n + \frac{h}{6}\left(\mathbf{k}_1 + 2\mathbf{k}_2 + 2\mathbf{k}_3 + \mathbf{k}_4\right).
$$

This provides a simple starting point for developing and testing the propagation framework.

## J2 Perturbation

The next stage introduces Earth's oblateness through the second zonal harmonic, $J_2$.

A perfectly spherical Earth is an approximation. In reality the Earth is slightly oblate, so its gravitational potential deviates from the ideal two-body model. The perturbing acceleration due to $J_2$ is

$$
\mathbf{a}_{J_2} =
\frac{3 J_2 \mu R_E^2}{2 r^5}
\begin{bmatrix}
x\left(5\dfrac{z^2}{r^2} - 1\right) \\
y\left(5\dfrac{z^2}{r^2} - 1\right) \\
z\left(5\dfrac{z^2}{r^2} - 3\right)
\end{bmatrix}
$$

and the total acceleration becomes

$$
\mathbf{a} = \mathbf{a}_{\text{2body}} + \mathbf{a}_{J_2}.
$$

This moves the project beyond the ideal two-body problem and towards effects that matter for real spacecraft orbit propagation.

## Validation

Physical and numerical checks are used to validate the propagator.

### Specific Orbital Energy

For the two-body problem, the specific orbital energy is

$$
\epsilon = \frac{v^2}{2} - \frac{\mu}{r}
$$

and should remain constant for an ideal two-body orbit. The implementation currently achieves relative energy errors on the order of $10^{-14}$, showing that the RK4 implementation behaves as expected for the selected timestep and test case.

### Specific Angular Momentum

The specific angular momentum is

$$
\mathbf{h} = \mathbf{r} \times \mathbf{v}.
$$

For two-body motion $\dot{\mathbf{h}} = \mathbf{0}$, so both the magnitude and direction of $\mathbf{h}$ should remain constant. This is used as an additional validation of the propagator.

With the introduction of $J_2$, these conservation properties are expected to change because the system is no longer a simple two-body problem. This provides a useful validation case for the perturbation model.

## Visualisation

MATLAB scripts are provided for analysing propagation results. For example, the propagated Cartesian states can be shown as a 3D orbit:

```matlab
plot3(x, y, z)
axis equal
grid on
xlabel('X [km]')
ylabel('Y [km]')
zlabel('Z [km]')
title('Spacecraft Trajectory')
```

Additional scripts examine:

- Orbital trajectory
- Specific angular momentum
- Energy error
- Numerical propagation error

MATLAB is used primarily for analysis and visualisation, while the core flight-dynamics implementation stays in C++.

## Building

### Requirements

- C++20 compatible compiler
- CMake ≥ 3.20
- Eigen
- Make or Ninja
- MATLAB (optional, for visualisation)

The project has been developed and tested on Apple Silicon macOS.

### CMake

```bash
cmake -S . -B build     # configure
cmake --build build     # build
./build/main            # run
```

### Make

A Makefile wraps the common workflow:

```bash
make run
```

This configures the project, builds it, and runs the executable.

## Design Philosophy

The project is developed incrementally. Rather than starting with a complete high-fidelity propagator, the software and physics are built up step by step, so each component can be validated before more complexity is added.

```text
Two-body dynamics
        ↓
RK4 numerical integration
        ↓
Validation & conservation tests
        ↓
J2 perturbation
        ↓
Additional perturbations
        ↓
Higher-fidelity orbit propagation
```

## Roadmap

### Completed

- [x] C++20 project setup
- [x] CMake build system
- [x] Eigen integration
- [x] Cartesian state vector
- [x] Two-body gravitational dynamics
- [x] RK4 propagator
- [x] Basic orbit propagation
- [x] MATLAB trajectory visualisation
- [x] Specific orbital energy validation
- [x] Specific angular momentum validation

### In Progress

- [ ] J2 gravitational perturbation
- [ ] J2 validation against analytical secular effects
- [ ] Improved automated unit tests
- [ ] Propagator interface for multiple force models

### Planned

- [ ] Higher-order numerical integrators
- [ ] Adaptive step-size integration
- [ ] Atmospheric drag
- [ ] Solar radiation pressure
- [ ] Third-body perturbations
- [ ] Earth orientation / reference-frame transformations
- [ ] Keplerian ↔ Cartesian state conversions
- [ ] Classical orbital element calculation
- [ ] Ground-track generation
- [ ] Numerical comparison against reference orbit propagators
- [ ] Higher-fidelity force modelling

## Motivation

This project is intended to build practical experience in spacecraft flight dynamics and orbit determination software, with a focus on both sides of the problem:

1. **The physics:** deriving and understanding the equations governing spacecraft motion.
2. **The software:** implementing those equations in maintainable, testable C++.

The long-term aim is a small but realistic flight-dynamics toolkit supporting orbit propagation, perturbation modelling, analysis, and eventually orbit-determination applications.

## Technologies

| Technology | Purpose                            |
| ---------- | ---------------------------------- |
| C++20      | Core flight-dynamics implementation |
| Eigen      | Linear algebra                     |
| CMake      | Build system                       |
| Make       | Build/run convenience              |
| MATLAB     | Analysis and visualisation         |
| GitHub     | Version control and development    |

## License

License information will be added as the project matures.