
% Seth Richards
% BIOEN 217 A
% 01/09/2020    
% Assignment 1


%Define Ebola Variables
month = 1:12;
sierraLeone = [80 258 180 478 1236 2932 1046 5778 1103 620 830 423];
liberia = [33 156 406 267 730 1503 691 832 477 208 112 115];
ebolaCases = [11628972 18378 7263;
              6190280 14964 5305;
              4503000 5530 2121];
          
          
% add new ebola cases to totals
newCases = [363; 756; 1005];
ebolaCases(:,2) = ebolaCases(:,2) + newCases;

% subtract corrected death toll estimates
newDeaths = [60; 23; 54];
ebolaCases(:,3) = ebolaCases(:,3) - newDeaths;

% correct population data
newPop = [1.5; 1; 0.7];
ebolaCases(:,1) = ebolaCases(:,1).*newPop;

% plot cases in SL per month && label data
plot(month,sierraLeone, 'b', 'LineWidth',2);
axis([0 12 0 7000]);
xlabel('Time (Months)');
ylabel('Ebola Cases');
title('Ebola Cases Per Month in Sierra Lione');
hold



% plot cases in L per month && label data
plot(month,liberia, 'r', 'LineWidth',2, 'MarkerEdgeColor','y');

% hightlight max val of SL, L
[maxSierra, iSierra] = max(sierraLeone);
[maxLiberia, iLiberia] = max(liberia);

plot(iSierra, maxSierra, 'mo');
plot(iLiberia, maxLiberia, 'blo');

% add a legend to graph
legend('sierraLione','liberia');

% end graph hold
hold off


