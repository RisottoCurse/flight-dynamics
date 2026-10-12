clear;
clc;
close all;

%% Constants

mu = 3.986004418e14;   % Earth's gravitational parameter [m^3/s^2]
Re = 6378137;          % Earth radius [m]

degToRad = pi / 180;

%% Keplerian Elements

a = 7000e3;            % Semi-major axis [m]
e = 0.1;               % Eccentricity
i = 30 * degToRad;     % Inclination [rad]
RAAN = 30 * degToRad;  % RAAN [rad]
omega = 45 * degToRad; % Argument of periapsis [rad]
nu0 = 0 * degToRad;    % Initial true anomaly [rad]

%% Generate orbital trajectory

N = 500;

trueAnomaly = linspace(0, 2*pi, N);

positionECI = zeros(3, N);

for k = 1:N

    [rECI, ~] = keplerianToECI( ...
        a, e, i, RAAN, omega, trueAnomaly(k), mu);

    positionECI(:, k) = rECI;

end

%% Initial satellite position

[r0, v0] = keplerianToECI( ...
    a, e, i, RAAN, omega, nu0, mu);

%% Convert metres to kilometres

positionECI = positionECI / 1000;
r0 = r0 / 1000;
Re = Re / 1000;

%% Plot

figure('Color', 'k');

hold on;
grid on;
axis equal;

% Earth
[X, Y, Z] = sphere(60);

surf(Re*X, Re*Y, Re*Z, ...
    'FaceColor', [0.2 0.5 0.9], ...
    'EdgeColor', 'none', ...
    'FaceAlpha', 0.7, ...
    'HandleVisibility', 'off');

% Orbit
plot3( ...
    positionECI(1,:), ...
    positionECI(2,:), ...
    positionECI(3,:), ...
    'r', 'LineWidth', 2, ...
    'DisplayName', 'Orbit');

% Satellite initial position
scatter3( ...
    r0(1), r0(2), r0(3), ...
    100, 'k', 'filled', ...
    'DisplayName', 'Satellite');

% ECI coordinate axes
axisLength = 1.5 * a / 1000;

quiver3(0,0,0, axisLength,0,0, ...
    'r', 'LineWidth', 1.5, 'HandleVisibility', 'off');

quiver3(0,0,0, 0,axisLength,0, ...
    'g', 'LineWidth', 1.5, 'HandleVisibility', 'off');

quiver3(0,0,0, 0,0,axisLength, ...
    'b', 'LineWidth', 1.5, 'HandleVisibility', 'off');

xlabel('ECI X [km]');
ylabel('ECI Y [km]');
zlabel('ECI Z [km]');

title('Satellite Orbit in ECI Reference Frame');

legend('Location', 'best');

view(45, 25);

lighting gouraud;
camlight headlight;