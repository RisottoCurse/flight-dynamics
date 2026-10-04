data = readtable("notebook/orbit_trajectory.csv");

t  = data.time;

x = data.x;
y = data.y;
z = data.z;

earthRadius = 6378.137e3;

radius = sqrt(x.^2 + y.^2 + z.^2);

altitude = radius - earthRadius;

figure;

plot(t / 60, altitude / 1000, ...
     'LineWidth', 1.5);

grid on;

xlabel('Time [min]');
ylabel('Altitude [km]');

title('Satellite Altitude');