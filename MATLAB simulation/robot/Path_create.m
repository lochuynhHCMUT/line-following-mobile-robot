function [X,Y] = Path_create
    % Line 1
    X1 = 3000:-300:500;
    Y1 = 0*X1;
 
    % Arc 1
    theta2 = 0:0.12:pi/2; % 0.12 = v/r*dt
    X2 = 500- 500*sin(theta2);
    Y2 = 500*(1-cos(theta2));
    % Arc 2
    theta3 = 0:0.12:pi/2;
    X3 = 500*(1-sin(pi/2+theta3));
    Y3 = 500*(1-cos(pi/2+theta3));
    
    %Line 2
    X4 = 500:168:668;
    Y4 = 1000+0*X4;
    
    %Arc3.1
    theta4 = 0:0.12:pi/4;
    X5 = 668+800*sin(theta4);
    Y5 = 1000+800*(1-cos(theta4));
    
    %Straight line Arc
    X6 = 1234.68:30:1265;
    Y6 = 1234.68:30:1265;
    
    %Arc3.2
    theta5 = 0:0.12:pi/4;
    X7 = 1265-800*(-sin(pi/4)+sin(pi/4-theta5));
    Y7 = 1265-800*(cos(pi/4)-cos(pi/4-theta5));
    
    %Last line
    X8 = 1800:300:3000;
    Y8 = 0*X8 +1500;
    

    X = [X1, X2, X3, X4, X5, X6, X7, X8];
    Y = [Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8];
%     hold on
     plot(X,Y)
end