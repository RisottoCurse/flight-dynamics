data = readtable("notebook/2BodyTests/orbit_trajectory.csv");

t  = data.time;

x = data.x;
y = data.y;
z = data.z;

figure;

plot3(x, y, z, 'LineWidth', 1.5);

hold on;

[xe, ye, ze] = sphere(50);

earthRadius = 6378.137e3;

surf(earthRadius * xe, ...
     earthRadius * ye, ...
     earthRadius * ze);

grid on;
axis equal;

xlabel('X [m]');
ylabel('Y [m]');
zlabel('Z [m]');

title('Two-Body Satellite Trajectory with 60 Degree Inclination');

hold off;