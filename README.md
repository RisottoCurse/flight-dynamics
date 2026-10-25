# Flight Dynamics

A C++20 flight-dynamics project for modelling, propagating, and manoeuvring spacecraft orbits using numerical integration, orbital mechanics, and increasingly realistic force models.

The project is a practical exploration of orbital mechanics, numerical methods, and spacecraft flight dynamics, with an emphasis on clean, modular, testable software.

## Features

- Cartesian spacecraft state representation
- Keplerian orbital elements and Cartesian ↔ Keplerian conversions
- Two-body gravitational dynamics and J2 perturbations
- Fourth-order Runge–Kutta (RK4) numerical propagation
- Impulsive manoeuvres and Hohmann transfers
- Eigen-based linear algebra and modular dynamics models
- Conservation checks for specific orbital energy and angular momentum
- MATLAB trajectory and orbital-element visualisation
- Catch2 tests

## Project Structure

```text
flight-dynamics/
├── inc/
│   ├── Common/
│   ├── Dynamics/
│   │   ├── TwoBodyDynamics.h
│   │   ├── OrbitalDynamics.h
│   │   └── J2Perturbation.h
│   ├── Manoeuvre/
│   │   ├── ImpulsiveManoeuvre.h
│   │   └── HohmannTransfer.h
│   ├── Propagator/
│   │   └── Propagator.h
│   └── State/
│       ├── State.h
│       └── KeplerianElements.h
├── src/
│   ├── Dynamics/
│   ├── Manoeuvre/
│   └── Propagator/
├── examples/
│   ├── example_twoBodyDynamics.cpp
│   └── example_HohmannTransfer.cpp
├── tests/
├── notebook/
├── external/
│   └── eigen/
├── CMakeLists.txt
├── Makefile
└── README.md
```

The tree summarises the main components; filenames may vary slightly with the repository's current organisation.

## Mathematical Model

### Two-Body Dynamics

The spacecraft state combines Cartesian position and velocity:

$$
\mathbf{x}=\begin{bmatrix}\mathbf{r}\\\mathbf{v}\end{bmatrix},\qquad
\mathbf{r}=\begin{bmatrix}x\\y\\z\end{bmatrix},\qquad
\mathbf{v}=\begin{bmatrix}v_x\\v_y\\v_z\end{bmatrix}.
$$

In the two-body approximation, the equations of motion are

$$
\dot{\mathbf{r}}=\mathbf{v},\qquad
\dot{\mathbf{v}}=-\frac{\mu}{r^3}\mathbf{r},\qquad r=\|\mathbf{r}\|.
$$

Here, $\mu$ is the central body's gravitational parameter.

### J2 Perturbation

Earth's oblateness introduces a perturbing acceleration given by

$$
\mathbf{a}_{J_2}=\frac{3J_2\mu R_E^2}{2r^5}
\begin{bmatrix}
x(5z^2/r^2-1)\\
y(5z^2/r^2-1)\\
z(5z^2/r^2-3)
\end{bmatrix}.
$$

The combined dynamics model uses $\mathbf{a}=\mathbf{a}_{\mathrm{2body}}+\mathbf{a}_{J_2}$ when J2 is enabled.

## Numerical Propagation

The classical fourth-order Runge–Kutta method advances the state over step size $h$:

$$
\begin{aligned}
\mathbf{k}_1 &= f(\mathbf{x}_n),\\
\mathbf{k}_2 &= f(\mathbf{x}_n+\tfrac{h}{2}\mathbf{k}_1),\\
\mathbf{k}_3 &= f(\mathbf{x}_n+\tfrac{h}{2}\mathbf{k}_2),\\
\mathbf{k}_4 &= f(\mathbf{x}_n+h\mathbf{k}_3),\\
\mathbf{x}_{n+1} &= \mathbf{x}_n+\frac{h}{6}(\mathbf{k}_1+2\mathbf{k}_2+2\mathbf{k}_3+\mathbf{k}_4).
\end{aligned}
$$

## Keplerian Elements and Orbital Manoeuvres

The software supports classical Keplerian elements $(a,e,i,\Omega,\omega,\nu)$ for orbit initialisation and analysis, together with Cartesian state conversion.

### Impulsive Manoeuvres

An ideal instantaneous burn changes velocity without changing position:

$$
\mathbf{r}^{+}=\mathbf{r}^{-},\qquad
\mathbf{v}^{+}=\mathbf{v}^{-}+\Delta\mathbf{v}.
$$

Manoeuvre execution is kept separate from numerical propagation.

### Hohmann Transfer

The Hohmann transfer is a two-impulse transfer between **coplanar circular orbits**, connected by an elliptical coast arc. For initial and target radii $r_1$ and $r_2$:

$$
a_t=\frac{r_1+r_2}{2}.
$$

The signed tangential burns follow from the vis-viva equation:

$$
\Delta v_1=\sqrt{\mu\left(\frac{2}{r_1}-\frac{1}{a_t}\right)}-\sqrt{\frac{\mu}{r_1}},
$$

$$
\Delta v_2=\sqrt{\frac{\mu}{r_2}}-\sqrt{\mu\left(\frac{2}{r_2}-\frac{1}{a_t}\right)}.
$$

Total impulse cost and transfer time are

$$
\Delta v_{\mathrm{total}}=|\Delta v_1|+|\Delta v_2|,\qquad
t_{\mathrm{transfer}}=\pi\sqrt{\frac{a_t^3}{\mu}}.
$$

The example transfers from a **7000 km circular orbit** to a **12000 km circular orbit**:

1. Create the initial state and compute analytical transfer parameters.
2. Apply the first prograde impulse to enter the transfer ellipse.
3. Propagate to apoapsis using RK4.
4. Apply the second impulse to circularise.
5. Propagate the final orbit and export the state history for MATLAB analysis.

For comparison with ideal analytical predictions, disable J2 during the transfer validation run.

## Validation

### Specific Orbital Energy

For unpowered two-body motion, specific orbital energy is conserved:

$$
\epsilon=\frac{v^2}{2}-\frac{\mu}{r}.
$$

The two-body propagator has demonstrated relative energy errors on the order of $10^{-14}$ in a selected test case.

### Specific Angular Momentum

Specific angular momentum is

$$
\mathbf{h}=\mathbf{r}\times\mathbf{v}.
$$

Its magnitude and direction remain constant under ideal two-body motion. Impulses can change energy and angular momentum instantaneously. Under J2 dynamics, the angular momentum direction may precess, and the two-body specific energy need not remain constant.

### Hohmann Transfer Checks

For the 7000 km → 12000 km example, the ideal transfer ellipse has

$$
a_t=9500\ \mathrm{km},\qquad e_t\approx0.26316.
$$

Numerical checks should verify the apoapsis radius, nearly zero radial velocity at apoapsis, and a nearly circular final orbit at 12000 km. Energy and angular momentum should remain constant on each unpowered two-body arc, with jumps at the burns.

## Visualisation

MATLAB is used for analysis and visualisation, while the numerical dynamics remain in C++. Scripts support:

- 3D ECI orbit trajectories
- Energy and angular-momentum diagnostics
- Keplerian element histories
- Hohmann transfer and initial/final orbit visualisation
- Markers for burn locations and comparisons between trajectory phases

## Building

### Requirements

- C++20-compatible compiler
- CMake ≥ 3.20
- Eigen
- Catch2 (for tests)
- Ninja or Make
- MATLAB (optional, for plots)

The project is developed on Apple Silicon macOS.

### CMake

```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/examples/example_HohmannTransfer
ctest --test-dir build --output-on-failure
```

The example executable name assumes the matching target is defined in `examples/CMakeLists.txt`.

### Make

A Makefile wraps common configuration and build workflows. Refer to the local Makefile for its supported run targets.

## Design Philosophy

Each numerical and physical capability is developed incrementally and validated before adding complexity:

```text
Two-body dynamics and RK4
        ↓
Conservation and regression tests
        ↓
J2 perturbation and Keplerian elements
        ↓
Impulsive manoeuvres
        ↓
Hohmann transfer planning and validation
        ↓
Higher-fidelity propagation and orbit targeting
```

## Roadmap

### Completed

- [x] C++20 project setup, CMake, and Eigen
- [x] Cartesian state representation
- [x] Two-body dynamics and RK4 propagation
- [x] MATLAB trajectory visualisation
- [x] Specific energy and angular-momentum checks
- [x] J2 perturbation implementation
- [x] Keplerian ↔ Cartesian conversions
- [x] Impulsive manoeuvres
- [x] Hohmann transfer calculations and example

### In Progress / Validation

- [ ] Extend Catch2 tests, especially full numerical Hohmann transfer tests
- [ ] Improve reusable trajectory logging
- [ ] Refine propagation to manoeuvre epochs and error checks

### Planned

- [ ] Plane-change and bi-elliptic transfers
- [ ] Lambert solver and orbit targeting
- [ ] Higher-order and adaptive-step integrators
- [ ] Atmospheric drag, solar radiation pressure, and third-body gravity
- [ ] Reference-frame transformations and Earth orientation
- [ ] Ground-track generation
- [ ] Comparisons against reference propagators
- [ ] Orbit-determination applications

## Motivation

This project builds practical experience in spacecraft flight dynamics and orbit-determination software, combining physical modelling with maintainable C++ implementation. Its long-term goal is a modular flight-dynamics toolkit for orbit propagation, manoeuvre planning, analysis, and eventually orbit determination.

## Technologies

| Technology | Purpose |
| --- | --- |
| C++20 | Core flight-dynamics implementation |
| Eigen | Linear algebra |
| Catch2 | Unit and integration tests |
| CMake | Build system |
| Make | Build and run convenience |
| MATLAB | Analysis and visualisation |
| GitHub | Version control and development |

## License

License information will be added as the project matures.
