% Seth Richards
% BIOEN 217 A
% 01/23/2020    
% Assignment 3

%% Part 1 - See Function Below

% Calculates the polar coordinates from cartesian inputs
[r,t] = cartesian2polar(3,4);

%% 2 -Importing Data

%imports file named samples.csv, specifically starting from the 2nd row and
%first column
filename = 'samples.csv';
data = csvread(filename, 2, 0);

%% 3 - Extracting Columns from a matrix

%extracts data from 2nd, 3rd, and 4th column of samples.csv
lead1 = data(:,2);
lead2 = data(:,3);
lead3 = data(:,4);

%% 4 - Plotting Revisited

%creates time array equal in length to column size in samples, and
%increments by 0.001
time = 1:length(lead1);
time = time .* 0.001;

%plots the patient data vs time in three subplots in the same figure
% figure(1)
% subplot(3,1,1)
% plot(time,lead1)
% 
% subplot(3,1,2)
% plot(time,lead2)
% 
% subplot(3,1,3)
% plot(time,lead3)

%% 5 - Curve Fitting

%imports all data from curvefittingdata.csv
filename2 = 'curvefittingdata.csv';
data = csvread(filename2, 0, 0);

%sets equal to col 1 and two from csv
xData = data(:,1);
yData = data(:,2);

%finds best fit polynomial of spoecified degree for data
pf1 = polyfit(xData,yData,1);
pf2 = polyfit(xData,yData,2);
pf3 = polyfit(xData,yData,3);
pf4 = polyfit(xData,yData,4);

%plots data of best fit polynomial and maps to xData
val1 = polyval(pf1,xData);
val2 = polyval(pf2,xData);
val3 = polyval(pf3,xData);
val4 = polyval(pf4,xData);

%plots all polynomials vs. original data on single plot

% figure(2)
% hold
% plot(xData,yData, 'o', 'LineWidth',2);
% xlabel('x)');
% ylabel('y');
% title('x vs. y');
% 
% plot(xData,val1, 'r', 'LineWidth',2);
% plot(xData,val2, 'g', 'LineWidth',2);
% plot(xData,val3, 'k', 'LineWidth',5);
% plot(xData,val4, 'm', 'LineWidth',2);
% legend('original','1st ord','2nd ord','3rd ord','4th ord');
%
% hold off



% Best fit is the 4th order equation


%finds roots of polynomial
roots4 = roots(pf4)

%plots original data vs best fit polynomial and roots on two subplots
figure(3)
sgtitle('Original Data vs. Poly Fit');

subplot(2,1,1);
plot(xData,yData,'o');

subplot(2,1,2);
hold
xlim([-10,10]);
plot(xData,val4, 'r', 'LineWidth',2);
plot(roots4,0,'ko');
legend('Best Fit','Root','Root','Root');

hold off



%% Function from Part 1
function [r,theta] = cartesian2polar(x,y)
%Cartesian2polar converts cartesian coordinates to polar coordinates
%   x is x coordinate
%    y is y coordinate
%   r is radius
%   theta is angle


    %equations from trig
    r = sqrt((x^2)+ (y^2)); 
    theta = atan(y/x);
end