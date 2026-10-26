clear;
clc;
close all;

%% ========================================
% GLOBAL PLOT SETTINGS
% ========================================

set(groot, 'defaultFigureColor', 'w');
set(groot, 'defaultAxesColor', 'w');

% Black axes and labels
set(groot, 'defaultAxesXColor', 'k');
set(groot, 'defaultAxesYColor', 'k');
set(groot, 'defaultAxesZColor', 'k');

% Fonts
set(groot, 'defaultAxesFontSize', 13);
set(groot, 'defaultAxesFontWeight', 'normal');
set(groot, 'defaultTextColor', 'k');

% Line appearance
set(groot, 'defaultLineLineWidth', 2);

% Grid
set(groot, 'defaultAxesXGrid', 'on');
set(groot, 'defaultAxesYGrid', 'on');
set(groot, 'defaultAxesZGrid', 'on');

% Opaque grid lines keep the background crisp without transparency blending.
set(groot, 'defaultAxesGridColor', [0.8 0.8 0.8]);
set(groot, 'defaultAxesGridAlpha', 1);

% Disable minor grids
set(groot, 'defaultAxesXMinorGrid', 'off');
set(groot, 'defaultAxesYMinorGrid', 'off');
set(groot, 'defaultAxesZMinorGrid', 'off');

%% ========================================
% CONSTANTS
% ========================================

mu = 3.986004418e14;    % [m^3/s^2]
Re = 6378137;           % Earth equatorial radius [m]

%% ========================================
% READ CSV
% ========================================

data = readtable('hohmann_transfer.csv');

time = data.time;

r = [data.x, data.y, data.z];
v = [data.u, data.v, data.w];

phase = data.phase;

% Convert positions to km
rKm = r / 1000;

% Position and velocity magnitudes
rNorm = vecnorm(r, 2, 2);
vNorm = vecnorm(v, 2, 2);

timeHours = time / 3600;

%% ========================================
% IDENTIFY ORBITAL PHASES
% ========================================

idxInitial  = phase == 0;
idxTransfer = phase == 1;
idxFinal    = phase == 2;

assert(any(idxInitial), 'Initial orbit phase missing.');
assert(any(idxTransfer), 'Transfer orbit phase missing.');
assert(any(idxFinal), 'Final orbit phase missing.');

initialIndex = find(idxInitial, 1, 'first');
finalIndex   = find(idxFinal, 1, 'first');

r1 = rNorm(initialIndex);
r2 = rNorm(finalIndex);

fprintf('\n===== HOHMANN TRANSFER =====\n');

fprintf('Initial orbital radius: %.3f km\n', r1/1000);
fprintf('Final orbital radius: %.3f km\n', r2/1000);

fprintf('Initial altitude: %.3f km\n', (r1-Re)/1000);
fprintf('Final altitude: %.3f km\n', (r2-Re)/1000);

%% ========================================
% CHECK EARTH INTERSECTION
% ========================================

if any(rNorm < Re)
    warning(['The spacecraft trajectory intersects Earth. ', ...
             'Check whether your C++ input uses orbital radius or altitude.']);
end

%% ========================================
% GENERATE INITIAL CIRCULAR ORBIT
% ========================================

r0 = r(initialIndex, :)';
v0 = v(initialIndex, :)';

e1 = r0 / norm(r0);

h0 = cross(r0, v0);
e3 = h0 / norm(h0);

e2 = cross(e3, e1);

theta = linspace(0, 2*pi, 1000);

initialOrbit = r1 * ...
    (e1*cos(theta) + e2*sin(theta));

initialOrbitKm = initialOrbit / 1000;

%% ========================================
% GENERATE EARTH SPHERE
% ========================================

[Xe, Ye, Ze] = sphere(120);

earthX = Re * Xe / 1000;
earthY = Re * Ye / 1000;
earthZ = Re * Ze / 1000;

%% ========================================
% CALCULATE ORBITAL PARAMETERS
% ========================================

% Specific orbital energy [J/kg]
specificEnergy = 0.5*vNorm.^2 - mu./rNorm;

% Specific angular momentum [m^2/s]
h = cross(r, v, 2);

hMagnitude = vecnorm(h, 2, 2);

% Semi-major axis [m]
semiMajorAxis = -mu ./ (2*specificEnergy);

% Eccentricity vector
eVector = cross(v, h, 2)/mu - r./rNorm;

eccentricity = vecnorm(eVector, 2, 2);

% Altitude [m]
altitude = rNorm - Re;

%% ========================================
% CREATE MAIN FIGURE WITH TABS
% ========================================

fig = figure( ...
    'Name', 'Hohmann Transfer Analysis', ...
    'NumberTitle', 'off', ...
    'Color', 'w', ...
    'Position', [100 100 1300 850]);

tabGroup = uitabgroup(fig);

tab1 = uitab(tabGroup, 'Title', '3D Trajectory');
tab2 = uitab(tabGroup, 'Title', 'Orbital Radius');
tab3 = uitab(tabGroup, 'Title', 'Orbital Altitude');
tab4 = uitab(tabGroup, 'Title', 'Keplerian Elements');
tab5 = uitab(tabGroup, 'Title', 'Orbital Energy');
tab6 = uitab(tabGroup, 'Title', 'Angular Momentum');

% Match the tab backgrounds to the white figure and axes backgrounds.
set([tab1 tab2 tab3 tab4 tab5 tab6], 'BackgroundColor', 'w');

%% ========================================
% TAB 1: HOHMANN TRANSFER TRAJECTORY
% ========================================

ax1 = axes('Parent', tab1);

hold(ax1, 'on');
grid(ax1, 'on');
axis(ax1, 'equal');

% Earth
surf(ax1, earthX, earthY, earthZ, ...
    'FaceColor', [0.25 0.55 0.85], ...
    'EdgeColor', 'none', ...
    'FaceAlpha', 0.85, ...
    'DisplayName', 'Earth');

% Initial circular orbit
plot3(ax1, ...
    initialOrbitKm(1,:), ...
    initialOrbitKm(2,:), ...
    initialOrbitKm(3,:), ...
    'b--', ...
    'LineWidth', 1.8, ...
    'DisplayName', 'Initial circular orbit');

% Transfer orbit
plot3(ax1, ...
    rKm(idxTransfer,1), ...
    rKm(idxTransfer,2), ...
    rKm(idxTransfer,3), ...
    'r', ...
    'LineWidth', 2.5, ...
    'DisplayName', 'Transfer orbit');

% Final circular orbit
plot3(ax1, ...
    rKm(idxFinal,1), ...
    rKm(idxFinal,2), ...
    rKm(idxFinal,3), ...
    'g', ...
    'LineWidth', 2.5, ...
    'DisplayName', 'Final circular orbit');

%% Manoeuvre locations

burn1Index = find(idxTransfer, 1, 'first');
burn2Index = find(idxFinal, 1, 'first');

plot3(ax1, ...
    rKm(burn1Index,1), ...
    rKm(burn1Index,2), ...
    rKm(burn1Index,3), ...
    'ko', ...
    'MarkerFaceColor', 'k', ...
    'MarkerSize', 9, ...
    'DisplayName', 'Burn 1');

plot3(ax1, ...
    rKm(burn2Index,1), ...
    rKm(burn2Index,2), ...
    rKm(burn2Index,3), ...
    'mo', ...
    'MarkerFaceColor', 'm', ...
    'MarkerSize', 9, ...
    'DisplayName', 'Burn 2');

%% ECI coordinate axes

axisLength = 1.15 * max(rNorm) / 1000;

% Negative and positive axes
plot3(ax1, [-axisLength axisLength], [0 0], [0 0], ...
    'r--', 'LineWidth', 1, 'HandleVisibility', 'off');

plot3(ax1, [0 0], [-axisLength axisLength], [0 0], ...
    'g--', 'LineWidth', 1, 'HandleVisibility', 'off');

plot3(ax1, [0 0], [0 0], [-axisLength axisLength], ...
    'b--', 'LineWidth', 1, 'HandleVisibility', 'off');

% Positive axis arrows
quiver3(ax1, 0,0,0,axisLength,0,0,0, ...
    'r', 'LineWidth', 2, ...
    'AutoScale', 'off', ...
    'HandleVisibility', 'off');

quiver3(ax1, 0,0,0,0,axisLength,0,0, ...
    'g', 'LineWidth', 2, ...
    'AutoScale', 'off', ...
    'HandleVisibility', 'off');

quiver3(ax1, 0,0,0,0,0,axisLength, ...
    'b', 'LineWidth', 2, ...
    'AutoScale', 'off', ...
    'HandleVisibility', 'off');

% Label each positive direction just beyond its arrow tip.
labelDistance = 1.04 * axisLength;
text(ax1, labelDistance,0,0,'+ECI X', ...
    'Color','r', 'FontWeight','bold');
text(ax1, 0,labelDistance,0,'+ECI Y', ...
    'Color','g', 'FontWeight','bold');
text(ax1, 0,0,labelDistance,'+ECI Z', ...
    'Color','b', 'FontWeight','bold');

%% Axis limits

maxRadius = max([rNorm; Re]);

plotLimit = 1.25 * maxRadius / 1000;

xlim(ax1, [-plotLimit plotLimit]);
ylim(ax1, [-plotLimit plotLimit]);
zlim(ax1, [-plotLimit plotLimit]);

xlabel(ax1, 'ECI X [km]');
ylabel(ax1, 'ECI Y [km]');
zlabel(ax1, 'ECI Z [km]');

% Match the grid-axis ticks and labels to the ECI arrows.
set(ax1, 'XColor', 'r', 'YColor', 'g', 'ZColor', 'b');

title(ax1, sprintf( ...
    'Hohmann Transfer: %.0f km to %.0f km Altitude', ...
    (r1-Re)/1000, ...
    (r2-Re)/1000));

legend(ax1, 'Location', 'bestoutside');

view(ax1, 35,25);

lighting(ax1, 'gouraud');
camlight(ax1, 'headlight');

%% ========================================
% TAB 2: ORBITAL RADIUS
% ========================================

ax2 = axes('Parent', tab2);

hold(ax2, 'on');
grid(ax2, 'on');

plot(ax2, timeHours, rNorm/1000, ...
    'LineWidth', 2);

yline(ax2, Re/1000, 'r--', ...
    'Earth Surface', ...
    'LineWidth', 1.5);

xlabel(ax2, 'Time [hours]');
ylabel(ax2, 'Orbital radius [km]');

title(ax2, 'Orbital Radius During Hohmann Transfer');

%% ========================================
% TAB 3: ORBITAL ALTITUDE
% ========================================

ax3 = axes('Parent', tab3);

hold(ax3, 'on');
grid(ax3, 'on');

plot(ax3, timeHours, altitude/1000, ...
    'LineWidth', 2);

yline(ax3, 0, 'r--', ...
    'Earth Surface', ...
    'LineWidth', 1.5);

xlabel(ax3, 'Time [hours]');
ylabel(ax3, 'Altitude [km]');

title(ax3, 'Spacecraft Altitude During Hohmann Transfer');

%% ========================================
% TAB 4: KEPLERIAN ELEMENTS
% ========================================

layout4 = tiledlayout(tab4, 2, 1, ...
    'TileSpacing', 'compact', ...
    'Padding', 'compact');

% Semi-major axis
ax4a = nexttile(layout4);

plot(ax4a, timeHours, semiMajorAxis/1000, ...
    'LineWidth', 2);

ylabel(ax4a, 'Semi-major axis [km]');
title(ax4a, 'Semi-major Axis Evolution');
grid(ax4a, 'on');

% Eccentricity
ax4b = nexttile(layout4);

plot(ax4b, timeHours, eccentricity, ...
    'LineWidth', 2);

xlabel(ax4b, 'Time [hours]');
ylabel(ax4b, 'Eccentricity');

title(ax4b, 'Eccentricity Evolution');
grid(ax4b, 'on');

%% ========================================
% TAB 5: SPECIFIC ORBITAL ENERGY
% ========================================

ax5 = axes('Parent', tab5);

hold(ax5, 'on');
grid(ax5, 'on');

plot(ax5, timeHours, specificEnergy/1e6, ...
    'LineWidth', 2);

xlabel(ax5, 'Time [hours]');
ylabel(ax5, 'Specific orbital energy [MJ/kg]');

title(ax5, 'Specific Orbital Energy During Hohmann Transfer');

%% ========================================
% TAB 6: SPECIFIC ANGULAR MOMENTUM
% ========================================

ax6 = axes('Parent', tab6);

hold(ax6, 'on');
grid(ax6, 'on');

plot(ax6, timeHours, hMagnitude/1e9, ...
    'LineWidth', 2);

xlabel(ax6, 'Time [hours]');
ylabel(ax6, 'Specific angular momentum [10^9 m^2/s]');

title(ax6, 'Specific Angular Momentum During Hohmann Transfer');

%% ========================================
% SELECT FIRST TAB
% ========================================

tabGroup.SelectedTab = tab1;

fprintf('\nMATLAB analysis complete.\n');
