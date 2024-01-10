data40 = readmatrix('E:\studyy\HK221\ProjectCDT\motorsit\motor2sit.csv');
time= data40(:,3)%/10^6;
e = data40(:,2);
plot (time, e*10^3);xlabel("time (s)");ylabel("e (mm)");
%setpoint = 100*ones(height(rpm),1);