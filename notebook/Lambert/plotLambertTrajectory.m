% Plot the trajectory exported by example_Lambert (positions in metres).

scriptDirectory = fileparts(mfilename('fullpath'));
csvFile = fullfile(scriptDirectory, 'lambert_transfer.csv');
if ~isfile(csvFile)
    error('Lambert:MissingData', ...
        'Run example_Lambert from the repository root first to generate %s.', csvFile);
end

data = readtable(csvFile);
requiredColumns = {'time', 'x', 'y', 'z', 'phase'};
assert(all(ismember(requiredColumns, data.Properties.VariableNames)), ...
    'CSV must contain time, x, y, z, and phase columns.');
transfer = data(data.phase == 0, :);
target = data(data.phase == 1, :);
assert(height(transfer) >= 2 && height(target) == 1, ...
    'Expected trajectory samples (phase 0) and one target (phase 1).');

r = [transfer.x, transfer.y, transfer.z];
targetPosition = [target.x, target.y, target.z];
assert(all(isfinite(r(:))) && all(isfinite(targetPosition(:))), ...
    'Position data must be finite.');
rKm = r / 1000;
targetKm = targetPosition / 1000;
arrivalError = norm(r(end, :) - targetPosition);
earthRadiusKm = 6378.137;

fig = figure('Name', 'Lambert Transfer', 'NumberTitle', 'off', 'Color', 'w');
ax = axes('Parent', fig);
hold(ax, 'on');
[xe, ye, ze] = sphere(80);
surf(ax, earthRadiusKm * xe, earthRadiusKm * ye, earthRadiusKm * ze, ...
    'FaceColor', [0.25 0.55 0.85], 'EdgeColor', 'none', ...
    'FaceAlpha', 0.7, 'DisplayName', 'Earth');
plot3(ax, rKm(:, 1), rKm(:, 2), rKm(:, 3), ...
    '-', 'Color', [0.85 0.2 0.15], 'LineWidth', 2, ...
    'DisplayName', 'Lambert transfer');
plot3(ax, rKm(1, 1), rKm(1, 2), rKm(1, 3), ...
    'go', 'MarkerFaceColor', 'g', 'MarkerSize', 8, ...
    'DisplayName', 'Departure');
plot3(ax, rKm(end, 1), rKm(end, 2), rKm(end, 3), ...
    'ks', 'MarkerSize', 9, 'LineWidth', 1.5, ...
    'DisplayName', 'Propagated arrival');
plot3(ax, targetKm(1), targetKm(2), targetKm(3), ...
    'mx', 'MarkerSize', 12, 'LineWidth', 2, 'DisplayName', 'Target');

axis(ax, 'equal');
grid(ax, 'on');
view(ax, 35, 25);
xlabel(ax, 'ECI X [km]');
ylabel(ax, 'ECI Y [km]');
zlabel(ax, 'ECI Z [km]');
title(ax, sprintf('Lambert transfer: %.1f min | Arrival error: %.3g m', ...
    (transfer.time(end) - transfer.time(1)) / 60, arrivalError));
legend(ax, 'Location', 'bestoutside');
rotate3d(fig, 'on');

fprintf('Time of flight: %.3f s\n', transfer.time(end) - transfer.time(1));
fprintf('Arrival position error: %.6g m\n', arrivalError);
