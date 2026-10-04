data = readtable("notebook/orbit_trajectory.csv");

t  = data.time;

x = data.x;
y = data.y;
z = data.z;
u = data.u;
v = data.v;
w = data.w;

position = [x, y, z];
velocity = [u, v, w];

angularMomentumVec = cross(position, velocity, 2);

angularMomentumMagnitude = vecnorm(angularMomentumVec, 2, 2);

initialAngularMomentum = angularMomentumMagnitude(1);

relativeAngularMomentumError = ...
    (angularMomentumMagnitude - initialAngularMomentum) ...
    / initialAngularMomentum;

figure;

plot(t / 60, angularMomentumMagnitude, ...
     'LineWidth', 1.5);

grid on;

xlabel('Time [min]');
ylabel('Specific angular momentum [m^2/s]');

title('Specific Angular Momentum');

%% Angular Momentum Error

figure;

plot(t / 60, relativeAngularMomentumError, ...
     'LineWidth', 1.5);

grid on;

xlabel('Time [min]');
ylabel('Relative angular momentum error');

title('Relative Specific Angular Momentum Error');

%% Max error

maxAngularMomentumError = ...
    max(abs(relativeAngularMomentumError));

fprintf('Maximum relative angular momentum error: %.3e\n', ...
        maxAngularMomentumError);


%% Checking Individual Components
figure;

plot(t / 60, angularMomentumVec(:,1), ...
     'LineWidth', 1.5);

hold on;

plot(t / 60, angularMomentumVec(:,2), ...
     'LineWidth', 1.5);

plot(t / 60, angularMomentumVec(:,3), ...
     'LineWidth', 1.5);

grid on;

xlabel('Time [min]');
ylabel('Specific angular momentum [m^2/s]');

title('Angular Momentum Components');

legend('h_x', 'h_y', 'h_z');

hold off;