function interactiveKeplerianOrbit()

    %% Constants

    mu = 3.986004418e14;
    Re = 6378137 / 1000;

    %% Initial Keplerian elements

    a = 7000e3;
    e = 0.1;
    i = 60;
    RAAN = 30;
    omega = 45;
    nu = 0;

    %% Create figure

    fig = uifigure( ...
        'Name', 'Keplerian Orbit Visualisation', ...
        'Position', [100 100 1100 750]);

    % 3D axes
    ax = uiaxes(fig, ...
        'Position', [50 180 700 520]);

    hold(ax, 'on');
    grid(ax, 'on');
    axis(ax, 'equal');

    xlabel(ax, 'ECI X [km]');
    ylabel(ax, 'ECI Y [km]');
    zlabel(ax, 'ECI Z [km]');

    view(ax, 45, 25);

    %% Earth

    [X, Y, Z] = sphere(50);

    surf(ax, Re*X, Re*Y, Re*Z, ...
        'FaceColor', [0.2 0.5 0.9], ...
        'EdgeColor', 'none', ...
        'FaceAlpha', 0.7);

    %% ECI Coordinate Axes

    axisLength = 25000;  % km

    % ECI X-axis
    quiver3(ax, ...
        0, 0, 0, ...
        axisLength, 0, 0, ...
        0, ...
        'Color', 'r', ...
        'LineWidth', 2, ...
        'MaxHeadSize', 0.15);

    % ECI Y-axis
    quiver3(ax, ...
        0, 0, 0, ...
        0, axisLength, 0, ...
        0, ...
        'Color', 'g', ...
        'LineWidth', 2, ...
        'MaxHeadSize', 0.15);

    % ECI Z-axis
    quiver3(ax, ...
        0, 0, 0, ...
        0, 0, axisLength, ...
        0, ...
        'Color', 'b', ...
        'LineWidth', 2, ...
        'MaxHeadSize', 0.15);

    % Axis labels
    text(ax, axisLength, 0, 0, 'ECI X', ...
        'Color', 'r', 'FontSize', 12);

    text(ax, 0, axisLength, 0, 'ECI Y', ...
        'Color', 'g', 'FontSize', 12);

    text(ax, 0, 0, axisLength, 'ECI Z', ...
        'Color', 'b', 'FontSize', 12);

    %% Orbit and satellite graphics

    orbitPlot = plot3(ax, nan, nan, nan, ...
        'r', 'LineWidth', 2);

    satellitePlot = scatter3(ax, nan, nan, nan, ...
        100, 'k', 'filled');

    %% Sliders

    sliderA = uislider(fig, ...
        'Position', [800 650 250 3], ...
        'Limits', [6500 20000], ...
        'Value', a/1000);

    sliderE = uislider(fig, ...
        'Position', [800 550 250 3], ...
        'Limits', [0 0.7], ...
        'Value', e);

    sliderI = uislider(fig, ...
        'Position', [800 450 250 3], ...
        'Limits', [0 180], ...
        'Value', i);

    sliderRAAN = uislider(fig, ...
        'Position', [800 350 250 3], ...
        'Limits', [0 360], ...
        'Value', RAAN);

    sliderOmega = uislider(fig, ...
        'Position', [800 250 250 3], ...
        'Limits', [0 360], ...
        'Value', omega);

    sliderNu = uislider(fig, ...
        'Position', [800 150 250 3], ...
        'Limits', [0 360], ...
        'Value', nu);

    %% Labels

    uilabel(fig, ...
        'Position', [800 670 250 20], ...
        'Text', 'Semi-major Axis [km]');

    uilabel(fig, ...
        'Position', [800 570 250 20], ...
        'Text', 'Eccentricity');

    uilabel(fig, ...
        'Position', [800 470 250 20], ...
        'Text', 'Inclination [deg]');

    uilabel(fig, ...
        'Position', [800 370 250 20], ...
        'Text', 'RAAN [deg]');

    uilabel(fig, ...
        'Position', [800 270 250 20], ...
        'Text', 'Argument of Periapsis [deg]');

    uilabel(fig, ...
        'Position', [800 170 250 20], ...
        'Text', 'True Anomaly [deg]');

    %% Callbacks

    sliders = [
        sliderA, sliderE, sliderI, ...
        sliderRAAN, sliderOmega, sliderNu
    ];

    for k = 1:numel(sliders)
        sliders(k).ValueChangedFcn = @(~,~) updateOrbit();
        sliders(k).ValueChangingFcn = @(~,event) ...
            updateOrbit(k, event.Value);
    end

    %% Initial plot

    updateOrbit();

    %% Nested update function

    function updateOrbit(changedSlider, changingValue)

        values = [
            sliderA.Value, sliderE.Value, sliderI.Value, ...
            sliderRAAN.Value, sliderOmega.Value, sliderNu.Value
        ];

        if nargin == 2
            values(changedSlider) = changingValue;
        end

        aCurrent = values(1) * 1000;
        eCurrent = values(2);

        iCurrent = deg2rad(values(3));
        RAANCurrent = deg2rad(values(4));
        omegaCurrent = deg2rad(values(5));
        nuCurrent = deg2rad(values(6));

        % Generate orbit
        N = 400;

        anomaly = linspace(0, 2*pi, N);

        positions = zeros(3,N);

        for j = 1:N

            [r, ~] = keplerianToECI( ...
                aCurrent, eCurrent, ...
                iCurrent, RAANCurrent, ...
                omegaCurrent, anomaly(j), mu);

            positions(:,j) = r / 1000;

        end

        % Satellite position
        [rSat, ~] = keplerianToECI( ...
            aCurrent, eCurrent, ...
            iCurrent, RAANCurrent, ...
            omegaCurrent, nuCurrent, mu);

        rSat = rSat / 1000;

        % Update orbit
        orbitPlot.XData = positions(1,:);
        orbitPlot.YData = positions(2,:);
        orbitPlot.ZData = positions(3,:);

        % Update satellite
        satellitePlot.XData = rSat(1);
        satellitePlot.YData = rSat(2);
        satellitePlot.ZData = rSat(3);

        % Keep Earth and the orbit visible
        plotLimit = max(Re, aCurrent * (1 + eCurrent) / 1000) * 1.15;

        xlim(ax, [-plotLimit plotLimit]);
        ylim(ax, [-plotLimit plotLimit]);
        zlim(ax, [-plotLimit plotLimit]);

        title(ax, sprintf( ...
            'a = %.0f km | e = %.2f | i = %.1f deg', ...
            aCurrent/1000, eCurrent, values(3)));

        drawnow limitrate;

    end

end