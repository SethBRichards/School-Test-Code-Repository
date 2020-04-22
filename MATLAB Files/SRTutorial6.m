% Seth Richards
% BIOEN 217 A
% 02/06/2020    
% Assignment 6


close all;clear all; clc;
%%  Activity - 1 Statistics

data = readtable('sampleData.xls');

% returns same value
[avg,std1] = findMean(data,3); 

std2 = std(data{:,3}); % a little different value
mean2 = mean(data{:,3});   
range(data{:,3});  

[max1,min1] = findMaxMin(data,3); 

max2 = max(data{:,3});
min2 = min(data{:,3});

prctile(data{:,9},[30 40 50]);

%% Activity - 2 Histograms

heightData = data{:,5};
% figure(1)
% histogram(heightData);

maxH = max(heightData);
minH = min(heightData);

weight = minH:5:maxH;

figure(2)
histogram(heightData,weight);
xlabel('Height');
ylabel('People');
title('Height vs. People STD');


%% Activity - 3 Bocplots

handLength = data{:,[6 9 8]};



figure(3)
hold
boxplot(handLength,'labels',{'Hand Breadth','Middle Finger Length','Index finger length'})
hold off

%% Activity - 4 Statistical Analysis


% My way
trials = [10 100 1000 10000 100000 1000000 10000000];
for k = 1:8    
    for i = 1:7
        pctDrops(k,i) = cross(trials(i));

    end
end


figure(4)
meansPct = mean(pctDrops);
stdPct = std(pctDrops); 

errorbar(1:7,meansPct,stdPct);
xlabel('Number of Drops (10^x)');
ylabel('Percent of Cross');
title('Number of Drops vs. Percent of Cross');
maxPct = max(meansPct);
minPct = min(meansPct);
xlim([minPct-0.03 maxPct+0.03])

%% matlab way

trials = [10 100 1000 10000 100000 1000000 10000000];
for k = 1:8    
    for i = 1:7
        pctDrops1(k,i) = crossM(trials(i));

    end
end

figure(5)
meansPct1 = mean(pctDrops1);
stdPct1 = std(pctDrops1); 


errorbar(1:7,meansPct1,stdPct1);
xlabel('Number of Drops (10^x)');
ylabel('Percent of Cross');
title('Number of Drops vs. Percent of Cross');
maxPct1 = max(meansPct1);
minPct1 = min(meansPct1);
ylim([minPct1-0.03 maxPct1+0.03])

% Matlab Way is significaantly faster





%% Matlab way to cross check
function [percent] = crossM(N)
   
    x0 = rand(N,1);
    y0 = rand(N,1);

    angle = 2*pi.*rand(N,1);
    x1 = x0 + cos(angle);
    y1 = y0 + sin(angle);
    
    total = sum((x1 > 1) | (x1 < 0) | (y1 > 1) | (y1 < 0));
    

    percent = total/N;
end


%% My way to cross check
function [percent] = cross(numDrops)
    needleP = [0;0];
    counter = 0;

    for k = 1:numDrops
       needleP = rand(2,1);
       angle = rand(1) * 2 * pi;
       needleE = [ needleP(1,1) + cos(angle); needleP(2,1) + sin(angle)];


       if (needleP(1,1) > 0 && needleE(1,1) < 0 || needleP(1,1) < 0 && needleE(1,1) > 0)
           counter = counter + 1;
       elseif(needleP(1,1) > 1 && needleE(1,1) < 1 || needleP(1,1) < 1 && needleE(1,1) > 1)
           counter = counter + 1;
       elseif(needleP(2,1) > 1 && needleE(2,1) < 1 || needleP(2,1) < 1 && needleE(2,1) > 1)
           counter = counter + 1;
       elseif(needleP(2,1) > 0 && needleE(2,1) < 0 || needleP(2,1) < 0 && needleE(2,1) > 0)
           counter = counter + 1;
       end

    end

    percent = counter/numDrops;
end

%% My way to find max and min
function [max,min] = findMaxMin(x,col)

    min = x{1,col};
    max = min;
    for k =1:size(x,1)
        if (x{k,col} > max)
            max = x{k,col};
        end
        if (x{k,col} < min)
            min = x{k,col};
        end
        
        
    end
    
end

%% My way to find avg, std
function [avg,std] = findMean(x,col)
    
    total = 0;
    stdHolder = 0;
    for k =1:size(x,1)
        total = total + x{k,col};
    end
    avg = total/size(x,1);
    
    for k =1:size(x,1)
        stdHolder = (x{k,col} - avg)^2 + stdHolder;
    end
    std = sqrt((stdHolder)/size(x,1));
end














