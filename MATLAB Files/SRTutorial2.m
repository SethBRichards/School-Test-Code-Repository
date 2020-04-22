
% Seth Richards
% BIOEN 217 A
% 01/16/2020    
% Assignment 2

%%
% 1 For Loops ----------------------------------------------------

%creates vector of 1 through 55 increpentimg by 6, then adds 3 to each
%value at each index
vector = 1:6:55;
for i = 1:length(vector)
    vector(i) = vector(i) + 3;
end

%creates multiplication table traditional way
multTable = zeros(10);
for m = 1:length( multTable(1,:) )
    for n = 1:length( multTable(:,1) )
        multTable(m,n) = m*n;
    end
end


%creates multiplication table matlab way
row = 1:10;
multTableOne = row.*(row');

%%

% 2 if-else ----------------------------------------------------
tax = 0;
income = input("Enter taxable income: ");

%initializes income array
randIncomes = rand(100,5).*190000+10000;
output = zeros(100,5);

for k = 1:100
    for j = 1:5
        output(k,j) = incomeTaxCalc(randIncomes(k,j)); %if statements in function far below
    end
end

%average income variables
avgCityIncome = mean(randIncomes); % Is each column a city? Tutorial language is confusing
avgIncome = mean(randIncomes, 'all'); % Is this what the tutorial wants?

%reducing max val of array

%traditional way
discardInitialArray = rand(50).*1.5;

for k = 1:50
    for i = 1:50
        if discardInitialArray(k,i) > 1.2
            discardInitialArray(k,i) = 0.8;
        end
    end
end
    
disp(max(discardInitialArray,[],'all')); %<-- Shows no value in array is
%greater than 1.2

%matlab way
inputArray = rand(50).*1.5;
outputArray = inputArray;% initializes array
outputArray(inputArray > 1.2) = 0.8;

disp(max(outputArray,[],'all')); %<-- Shows no value in array is
%greater than 1.2


%%

% 3 While Loops ----------------------------------------------------

%blast off function (counts down from input and says blast off
pause on % to enable pause function

countdown = input("Countdown from: ");
while countdown >= 0
    disp(countdown);
    pause(0.5)
    countdown = countdown - 1;
end   
disp("Blast Off!");

%%

% 4 Solving Systems of Linear Equations ----------------------------------------------------

%Plots the two equations
A = [2 3; 3 4];
b = [6 12]';
x = A \ b;

t = -10:30;
f1 = t.*-2/3 + 2; % 2x+3y=6
f2 = t.*-3/4 + 3; % 3x+4y=12

plot(t,f1, 'b', 'LineWidth',2);
axis([-10 30 -20 20]);
xlabel('x)');
ylabel('y');
title('x vs. y');
hold
plot(t,f2, 'r', 'LineWidth',2);
legend('2x+3y=6','3x+4y=12');
plot(x(1),x(2),'go');
hold off

%Lines do intersect in graph

%solving system of equations(three equations)
C = [2 3 4; 3 1 -2; 1 -2 3];
xx = [10 5 8]';
solution = C \ xx;



%%

% 6 Solving for Roots of Polynomials ----------------------------------------------------

%defines polynomial values
P1 = [-3 6];
P2 = [1 -1 -2];
P3 = [6 -5 -29 10];
P4 = [6 -41 16 209 -70];
P5 = [24 -106 -1173 4473 -4954 1680];

%creates functions
ti = -10:0.1:10;
y1 = polyval(P1, ti);
y2 = polyval(P2,ti);
y3 = polyval(P3,ti);
y4 = polyval(P4,ti);
y5 = polyval(P5,ti);


%finds roots of functions
rootsP1 = roots(P1);
rootsP2 = roots(P2);
rootsP3 = roots(P3);
rootsP4 = roots(P4);
rootsP5 = roots(P5);
disp(rootsP4);

%plots functions
plot(ti,y1, 'b', 'LineWidth',2);
axis([-10 10 -400 400]);
xlabel('x)');
ylabel('y');
title('x vs. y');
hold
plot(ti,y2, 'b', 'LineWidth',2);
plot(ti,y3, 'b', 'LineWidth',2);
plot(ti,y4, 'b', 'LineWidth',2);
plot(ti,y5, 'b', 'LineWidth',2);
legend('y1','y2','y3','y4','y5');

%plots roots on x-axis
plot(rootsP1,0,'go');
plot(rootsP2,0,'go');
plot(rootsP3,0,'go');
plot(rootsP4,0,'go');
plot(rootsP5,0,'go');

hold off




%%

%Function for part 2

function tax = incomeTaxCalc(income)
   
    if income>150000
        tax = 0.3;
    elseif income >60000
        tax = 0.2;
    elseif income > 20000
        tax = 0.1;
    elseif income > 1
        tax = 0.05;
    else
        disp('You have no taxable income');
    end
end