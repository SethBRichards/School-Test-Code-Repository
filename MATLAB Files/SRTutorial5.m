% Seth Richards
% BIOEN 217 A
% 02/06/2020    
% Assignment 5


close all;clear all; clc;
%% Activity - 1


[t,y] = ode45(@myODE,[0 10],0);

% plots solution of the ode t to the power of sin(t)
plot(t,y, 'b', 'LineWidth',2);
xlabel('T)');
ylabel('Y');
title('ODE of t to the power of sin(t)');

%% Activity - 2
r = 0.05;
K = 100;

% calculates solution of the odes with single r and K parameter
[t1,p] = ode45(@(t,y) logisticODE(t,y,r,K),[0 500],10);



%% Cell Arrays - 3

r = [0.01 0.02 0.05 0.1];

t2 = {};
p2 = {};

% gets the ODE solutions for all values of r
for k = 1:4
    rCurrent = r(k);
    [t2{k},p2{k}] = ode45(@(t,y) logisticODE(t,y,rCurrent,K),[0 500],10)
end

% plots solution of the ode for all r values
figure(2)
hold
plot(t2{1,1},p2{1,1}, 'k', 'LineWidth',2);
xlabel('t');
ylabel('P');
title('P vs. t for several r values');

plot(t2{1,2},p2{1,2}, 'b', 'LineWidth',2);
plot(t2{1,3},p2{1,3}, 'r', 'LineWidth',2);
plot(t2{1,4},p2{1,4}, 'm', 'LineWidth',2);
legend('r = 0.01','r = 0.02','r = 0.05','r = 0.1');

hold off

%% System of Differential Equations - 4

[t3,y3] = ode45(@(t,y) lotkaVolterra(t,y),[0 50],[50 ; 10]);

% plots the solution to the predator vs. prey graphs
figure(3)
hold
plot(t3,y3(:,1), 'm', 'LineWidth',2);
xlabel('t');
ylabel('P and V');
title('Predators and Prey vs. t');
plot(t3,y3(:,2), 'b', 'LineWidth',2);
legend('Prey','Predators');

hold off

%% Second Order Differential Equations - 5

p = [1,0.5,1];
[t4,y4] = ode45(@(t,y) springMass(t,y,p),[0 50],[0.1 ; 0]);

% plots the ode solution and y double prime solution vs t
figure(4)
hold
plot(t4,y4(:,1), 'm', 'LineWidth',2);
xlabel('t');
ylabel('P and V');
title('Y and Yprime Spring Mass vs. t');
plot(t4,y4(:,2), 'b', 'LineWidth',2);
legend('y(t)','yrime(t)');

hold off


%%

function yprimes = springMass(t,y,p)
% This function evaluates the derivative of the function 
% p(1) = m, p(2) = c, p(3) = k
    
    ydoubleprime = -y(2) *(p(2)/p(1)) - y(1) * (p(3)/p(1));
    yprime = y(2);
    yprimes = [yprime ; ydoubleprime];

end


%%

function yprime = lotkaVolterra(t,y)
% This function evaluates the derivative of the function
    y1prime = (0.5 * y(1)) - (0.02 * y(1) * y(2)); % V
    y2prime = (-0.4 * y(2)) + (0.01 * y(1) * y(2)); % P
    yprime = [y1prime;y2prime];

end
%%

function yprime = logisticODE(t,y,r,K)
% This function evaluates the derivative of the function
    yprime = (r * y) * (1 - (y/K));
end

%%
function yprime = myODE(t,y)
% This function evaluates the derivative of the function
    yprime = t.^sin(t);
end

