data40 = readmatrix('E:\studyy\HK221\ProjectCDT\motorsit\checklinear(GM25+TB6612)\40.csv');
time= data40(:,3);
rpm = data40(:,2);
plot (time, rpm)
title('Step response - Setpoint 100');xlabel('Time (s)');ylabel('RPM');