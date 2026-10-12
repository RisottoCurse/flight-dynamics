function [rECI, vECI] = keplerianToECI(a, e, i, RAAN, omega, nu, mu)

    % Inputs:
    % a     - Semi-major axis [m]
    % e     - Eccentricity [-]
    % i     - Inclination [rad]
    % RAAN  - Right ascension of ascending node [rad]
    % omega - Argument of periapsis [rad]
    % nu    - True anomaly [rad]
    % mu    - Gravitational parameter [m^3/s^2]

    % Semi-latus rectum
    p = a * (1 - e^2);

    % Orbital radius
    r = p / (1 + e * cos(nu));

    % Position in perifocal frame
    rPQW = r * [
        cos(nu);
        sin(nu);
        0
    ];

    % Velocity in perifocal frame
    vPQW = sqrt(mu / p) * [
        -sin(nu);
        e + cos(nu);
        0
    ];

    % Rotation about Z-axis (RAAN)
    R3_RAAN = [
        cos(RAAN), -sin(RAAN), 0;
        sin(RAAN),  cos(RAAN), 0;
        0,          0,         1
    ];

    % Rotation about X-axis (Inclination)
    R1_i = [
        1, 0,       0;
        0, cos(i), -sin(i);
        0, sin(i),  cos(i)
    ];

    % Rotation about Z-axis (Argument of periapsis)
    R3_omega = [
        cos(omega), -sin(omega), 0;
        sin(omega),  cos(omega), 0;
        0,           0,          1
    ];

    % PQW to ECI transformation
    Q = R3_RAAN * R1_i * R3_omega;

    % Convert to ECI
    rECI = Q * rPQW;
    vECI = Q * vPQW;

end