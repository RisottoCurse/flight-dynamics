clear;
clc;
close all;

%% Load propagation data

data = readmatrix("notebook/J2Tests/j2_orbit_trajectory.csv", ...
    "NumHeaderLines", 1);

time = data(:, 1);

position = data(:, 2:4);
velocity = data(:, 5:7);

%% Calculate RAAN

raan = zeros(length(time), 1);

for k = 1:length(time)
    r = position(k, :)';
    v = velocity(k, :)';
    h = cross(r, v);
    n = cross([0; 0; 1], h);
    raan(k) = atan2(n(2), n(1));
end

%% Unwrap RAAN

raan = unwrap(raan);

raanDegrees = rad2deg(raan);

timeDays = time / 86400;

%% Fit a straight line

fitCoefficients = polyfit(timeDays, raanDegrees, 1);

numericalRate = fitCoefficients(1);

%% Analytical J2 rate

mu = 3.986004418e14;
Re = 6.3781363e6;
J2 = 1.08262668e-3;

a = 7.0e6;
e = 0.0;

inclination = deg2rad(60.0);

p = a * (1 - e^2);

nMeanMotion = sqrt(mu / a^3);

analyticalRate = ...
    -(3/2) * J2 * nMeanMotion ...
    * (Re / p)^2 ...
    * cos(inclination);

analyticalRate = rad2deg(analyticalRate) * 86400;

%% Display results

fprintf("Numerical RAAN rate:   %.6f deg/day\n", ...
    numericalRate);

fprintf("Analytical RAAN rate:  %.6f deg/day\n", ...
    analyticalRate);

fprintf("Difference:            %.6f deg/day\n", ...
    numericalRate - analyticalRate);

fprintf("Relative error:        %.6e\n", ...
    abs((numericalRate - analyticalRate) / analyticalRate));

fprintf("Initial RAAN: %.6f deg\n", ...
    rad2deg(raan(1)));
    
fprintf("Final RAAN:   %.6f deg\n", ...
    rad2deg(raan(end)));

%% Plot

figure;

plot(timeDays,raanDegrees,"LineWidth", 1.5);

hold on;

plot(timeDays,polyval(fitCoefficients, timeDays), "--", "LineWidth", 1.5);

grid on;

xlabel("Time [days]");
ylabel("RAAN [deg]");

title("J2-Induced RAAN Precession");

legend("Numerical", "Linear fit", "Location","best");