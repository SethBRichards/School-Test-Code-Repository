% Seth Richards
% BIOEN 217 A
% 01/30/2020    
% Assignment 4


close all;clear all; clc;
%% 1 - Numerical Integration


%step size 0.1
step = 0.1;
x = 0:step:4;

y = exp(x);
sum = LRS(x,y);
%fairly close

%integration technique
area = integral (@exp,0,5);

% This method is much basically dead on compared to the real version and
% more accurate than my rienmann sum, though it'd be closer with a
% different step size

%% 2 - Numerical Differentiation

step = 0.1;
x = 0:step:4;

%derivative quality increases as step size decreases
dif = derive(x,y);

%% 3 - Function Handles

xy1 = [0 0];
xy2 = [5 80];

% calcualtes distance between two points of any dimension
distanceCalculator = @(x,y) sqrt(sum(((y-x).^2),'all')); 

distanceCalculator(xy1,xy2)

%% 4 - Using Fmin Search

x = [-1.2,1];
rosenBrockBanana = @(x) 100*(x(2)-(x(1)^2)) + (1 - x(1))^2;

searchedMin = fminsearch(rosenBrockBanana,x);
calculatedVal = [-1.2279 -1.2137] .*10^44; %value from search

rosenBrockBanana(calculatedVal); %checking min value

%% 5 - Nlinfit

fearData = xlsread('trepidation.xls');
x = fearData(1,:);
y = fearData(2,:);

%my guess for parameters
beta0 = [87 -0.08];

%returns fitted parameters
beta = nlinfit(x,y,@decayingExponential,beta0);

%plots original data vs. nlinfit parameter data
figure(1)
title('Original Data vs. nlinfit');
hold
plot(x,y,'o');
plot(x,decayingExponential(beta,x), 'r', 'LineWidth',2);
legend('Original Data','Nlinfit');

hold off

%% 6 - Simulation

westWall = 0;
eastWall = 10;
x0 = 0;

results = zeros(1,25000);
for i = 1:length(results)
    location = x0;
    steps = 0;
    while location<10
        steps = steps + 1;
        location = location + rand(1);
    end
    results(i) = steps;
    
end

histogram(results);
%looks like a bell curve
% minimum to look about the same is roughly 25,000 trials

%% 7 - Check Off Activity

%step size 0.1
step = 0.1;
x = 0:step:4;

y = exp(x);
sum = RRS(x,y);
%similar for LRS but uses y(n+1) instead of y(1)

gaussian = @(c,x) c(1)* exp((-(x-c(2)).^2)/c(3).^2);
betaGuess = [1 2 3];
betaFit = nlinfit(xdata,ydata,gaussian,betaGuess);
% betaFit = 3.1965 0.4007 0.768
estimatedY = gaussian(betaFit,xdata);

areaEst1 = integral(@(x)gaussian(betaFit,x),-2,3); % = 4.3512
areaEst2 = RRS(xdata,estimatedY); % = 4.3452

% estimates are very close together, but I would assume that the integrate function is
% a better estimate than the right rienmann sum becuase it's optimized
% while my DIY sum function uses a system that's less complicated


%% Decaying Exponential Function
function y = decayingExponential(b,x)
    %x is time data
    %b is constants parameter
    y = b(1)* exp(x*b(2));
end

%%

% returns a value at each point representing the derivative at that point
function [derivative] = derive(x,y)
    
    %delta x vs delta y
    deltaX = diff(x);
    deltaY = diff(y);
    derivative = deltaY./deltaX;


end


%%
% right rienmann sum
function [sum] = RRS(x,y)
    
    sum = 0;
    for i = 1:(length(x)-1)
        sum = (x(i+1)-x(i)) * y(i+1) + sum;
    end


end

%%
% left rienmann sum
function [sum] = LRS(x,y)
    
    sum = 0;
    for i = 1:(length(x)-1)
        sum = (x(i+1)-x(i)) * y(i) + sum;
    end


end