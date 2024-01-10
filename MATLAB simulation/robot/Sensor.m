function error = Sensor(x,y,theta)
    % Robot paremeter
    L = 200; %distance from motor axis midpoint to sensors module midpoint
    L_sensor = 110; %sensor detection range

    % Find intersections
    [X,Y] = Path_create;
    sensor_p = [x + L*cos(theta), y + L*sin(theta)];
    %sensor_p = [x,y];
    X_sensor = linspace(sensor_p(1) + (L_sensor/2)*cos(theta+pi/2), sensor_p(1) + (L_sensor/2)*cos(theta-pi/2), size(X,2));
    Y_sensor = linspace(sensor_p(2) + (L_sensor/2)*sin(theta+pi/2), sensor_p(2) + (L_sensor/2)*sin(theta-pi/2), size(Y,2));
    P = InterX([X;Y],[X_sensor;Y_sensor]);
    error_real = sqrt((sensor_p(1) - P(1))^2 + (sensor_p(2) - P(2))^2);
    error = error_real*sign((sensor_p(1) - P(1))*(sensor_p(1) + (L_sensor/2)*sin(theta)-P(1)));

end