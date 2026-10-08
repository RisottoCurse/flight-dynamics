data = readtable("notebook/orbit_trajectory.csv");

t  = data.time;

x = data.x;
y = data.y;
z = data.z;
u = data.u;
v = data.v;
w = data.w;

mu = 3.986004418e14;

speed = sqrt(u.^2 + v.^2 + w.^2);

specificEnergy = ...
    0.5 * speed.^2 ...
    - mu ./ radius;

initialEnergy = specificEnergy(1);

relativeEnergyError = ...
    (specificEnergy - initialEnergy) ...
    / abs(initialEnergy);

figure;

plot(t / 60, specificEnergy, ...
     'LineWidth', 1.5);

grid on;

xlabel('Time [min]');

ylabel('Specific orbital energy [J/kg]');

title('Specific Orbital Energy');

%% Relative energy error

figure;

plot(t / 60, relativeEnergyError, ...
     'LineWidth', 1.5);

grid on;

xlabel('Time [min]');

ylabel('Relative energy error');

title('Relative Specific Energy Error');

%% Print validation result

maxEnergyError = max(abs(relativeEnergyError));

fprintf('Maximum relative energy error: %.3e\n', ...
        maxEnergyError);