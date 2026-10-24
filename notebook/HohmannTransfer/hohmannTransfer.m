clear;
clc;
close all;

%% ========================================
% Plot settings
% ========================================

set(groot, 'defaultFigureColor', 'w');
set(groot, 'defaultAxesColor', 'w');

set(groot, 'defaultAxesXColor', 'k');
set(groot, 'defaultAxesYColor', 'k');
set(groot, 'defaultAxesZColor', 'k');

set(groot, 'defaultAxesFontSize', 13);
set(groot, 'defaultLineLineWidth', 1.8);

%% ========================================
% Constants
% ========================================

mu = 3.986004418e14;

Re = 6378137;

%% ========================================
% Read CSV
% ========================================

data = readtable('notebook/HohmannTransfer/hohmann_transfer.csv');

time = data.time;

x = data.x;
y = data.y;
z = data.z;

u = data.u;
v = data.v;
w = data.w;

phase = data.phase;

r = [x, y, z];
v = [u, v, w];

rNorm = vecnorm(r, 2, 2);
vNorm = vecnorm(v, 2, 2);

%% ========================================
% 1. Orbital trajectory
% ========================================

figure('Name', 'Hohmann Transfer Trajectory');

hold on;
grid on;
axis equal;

% Earth sphere

[Xe, Ye, Ze] = sphere(80);

surf(Re * Xe / 1000, ...
     Re * Ye / 1000, ...
     Re * Ze / 1000, ...
     'FaceColor', [0.3 0.6 0.9], ...
     'EdgeColor', 'none', ...
     'FaceAlpha', 0.8);

% Transfer orbit

idxTransfer = phase == 1;
idxFinal = phase == 2;

plot3(x(idxTransfer)/1000, ...
      y(idxTransfer)/1000, ...
      z(idxTransfer)/1000, ...
      'r', 'LineWidth', 2);

plot3(x(idxFinal)/1000, ...
      y(idxFinal)/1000, ...
      z(idxFinal)/1000, ...
      'g', 'LineWidth', 2);

% Initial circular orbit reference

theta = linspace(0, 2*pi, 500);

r1 = 7000;

plot3(r1*cos(theta), ...
      r1*sin(theta), ...
      zeros(size(theta)), ...
      'b--', 'LineWidth', 1.5);

% Initial burn position

plot3(x(1)/1000, ...
      y(1)/1000, ...
      z(1)/1000, ...
      'ko', ...
      'MarkerFaceColor', 'k', ...
      'MarkerSize', 7);

% Second burn position

idxBurn2 = find(phase == 2, 1);

plot3(x(idxBurn2)/1000, ...
      y(idxBurn2)/1000, ...
      z(idxBurn2)/1000, ...
      'mo', ...
      'MarkerFaceColor', 'm', ...
      'MarkerSize', 7);

xlabel('ECI X [km]');
ylabel('ECI Y [km]');
zlabel('ECI Z [km]');

title('Hohmann Transfer: 7000 km to 12000 km');

legend('Earth', ...
       'Transfer orbit', ...
       'Final orbit', ...
       'Initial orbit', ...
       'Burn 1', ...
       'Burn 2', ...
       'Location', 'bestoutside');

view(35, 25);

%% ========================================
% 2. Keplerian elements
% ========================================

specificEnergy = 0.5 * vNorm.^2 - mu ./ rNorm;

semiMajorAxis = -mu ./ (2 * specificEnergy);

h = cross(r, v, 2);

hNorm = vecnorm(h, 2, 2);

eVector = cross(v, h, 2)/mu - r ./ rNorm;

eccentricity = vecnorm(eVector, 2, 2);

figure('Name', 'Orbital Elements');

subplot(2,1,1);

plot(time/3600, semiMajorAxis/1000);

ylabel('Semi-major axis [km]');
title('Semi-major Axis Evolution');

grid on;

subplot(2,1,2);

plot(time/3600, eccentricity);

xlabel('Time [hours]');
ylabel('Eccentricity');

title('Eccentricity Evolution');

grid on;

%% ========================================
% 3. Specific orbital energy
% ========================================

figure('Name', 'Specific Orbital Energy');

plot(time/3600, specificEnergy/1e6);

xlabel('Time [hours]');
ylabel('Specific energy [MJ/kg]');

title('Specific Orbital Energy');

grid on;

%% ========================================
% 4. Angular momentum
% ========================================

figure('Name', 'Angular Momentum');

plot(time/3600, hNorm/1e9);

xlabel('Time [hours]');
ylabel('Specific angular momentum [10^9 m^2/s]');

title('Angular Momentum Evolution');

grid on;