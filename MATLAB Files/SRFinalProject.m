% Seth Richards
% BIOEN 217 A
% 03/13/2020    
% Final Project

close all;clear all; clc;
%%

zoomXL = [-1.75,-1.1,-0.9, -0.5];
zoomXU = [0.95,0.05,-0.1, -0.4];

zoomYL = [-1.75,-1.4,-1.0, -0.8];
zoomYU = [0.95,0.55,0.2,-0.3];

scale = [0.00035, 0.0008, 0.00019,0.00002];
for k = 1:4
    pause(5)
    [X, Y] = meshgrid( zoomXL(k):scale(k):zoomXU(k) , zoomYL(k):scale(k):zoomYU(k) );
    s = X + 1j*Y;
    aa = mandleCheck(s)*1.5;
    figure(k)
    colormap(gray);
    imagesc(aa);
    xlabel('Real Axis)');
    ylabel('Imaginary Axis');
    title('Mandlebrot Plot');
    if k <= 5
        xticklabels = zoomXL(k):zoomXU(k);
        xticks = linspace(1, size(aa, 2), numel(xticklabels));
        set(gca, 'XTick', xticks, 'XTickLabel', xticklabels);
        yticklabels = zoomYL(k):zoomYU(k);
        yticks = linspace(1, size(aa, 1), numel(yticklabels));
        set(gca, 'YTick', yticks, 'YTickLabel', flipud(yticklabels(:)))
    end
end
% [X1, Y1] = meshgrid( -3:0.0004:0.5 , -2:0.0004:0 );
% s = X1 + 1j*Y1;
% aa = mandleCheck(s) * 1.5;
% 
% figure(1)
% imagesc(aa);
% xticklabels = -2:0.5:2;
% xticks = linspace(1, size(aa, 2), numel(xticklabels));
% set(gca, 'XTick', xticks, 'XTickLabel', xticklabels)
% yticks = linspace(1, size(aa, 1), numel(xticklabels));
% set(gca, 'YTick', yticks, 'YTickLabel', flipud(xticklabels(:)))

% [X2, Y2] = meshgrid( -1:0.0001:0 , -0.75:0.0001:0 );
% s = X2 + 1j*Y2;
% aa = mandleCheck(s) * 1.5;
% 
% figure(1)
% imagesc(aa);
% xticklabels = -2:0.5:2;
% xticks = linspace(1, size(aa, 2), numel(xticklabels));
% set(gca, 'XTick', xticks, 'XTickLabel', xticklabels)
% yticks = linspace(1, size(aa, 1), numel(xticklabels));
% set(gca, 'YTick', yticks, 'YTickLabel', flipud(xticklabels(:)))
%%

function [imageArray] = mandleCheck(s)

    imageArray = zeros(size(s));
    n = 0;
    for k = 1:20
        n = (n.^2) + s;
        imageArray = imageArray > 1 | abs(n) > 700; %6.4032;
    end

end




