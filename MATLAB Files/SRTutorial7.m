% Seth Richards
% BIOEN 217 A
% 02/20/2020    
% Assignment 7


close all;clear all; clc;
%% 1 & 2
 A = imread('BPAEC.jpg');

imagesc(A), axis equal;

t = 1:721;

%% 3 - RGB image as a stack

for s = 1:3
    figure(2);
    colormap('gray');
    
    subplot(2,2,s)
    imagesc(A(:,:,s))
    
end

subplot(2,2,4)
imagesc(A(:,:,:))


%% 4 - Intensity scaling and contrast enhancement


figure(3)
B = mean(A,3);
image(B);

figure(4)
colormap('gray');

imagesc(B);

red = A(:,:,1);
green = A(:,:,2);
blue = A(:,:,3);

[yRed, x] = imhist(red);
[yGreen] = imhist(green);
[yBlue] = imhist(blue);

figure(4)
bar(x,yRed,'r'); hold on
bar(x,yGreen,'g');
bar(x,yBlue,'b');
hold off

figure(5)
colormap('gray');
C = myImagesc(B);
image(C);


%% 5 Thresholding

D = imread('im.tif');

thresholdedImage = D > 0.01;
figure(6)
for i = min(D,[],'all'):max(D,[],'all')
    pause(0.05);
    colormap('gray');
    thresholdedImage = D > i;
    imagesc(thresholdedImage);
    
end

figure(7)
chosenThreshIm = D > 100;
chosenThreshIm = uint8(chosenThreshIm);
productIm = chosenThreshIm .* D;
colormap('gray');
imagesc(productIm);

%% 6 - Median Filtering


E = imread('WristXRaySP.tif');
figure(8);
colormap('gray');
image(E);

for i = 2:902 % rows
    for j = 2: 655 % columns
        
        newImage(i,j) = median([E(i-1,j-1) E(i,j-1) E(i+1,j-1) E(i-1,j)  E(i-1,j+1) E(i+1,j+1) E(i,j) E(i,j+1)  E(i+1,j) ]);
          
    end
end

figure(9)
colormap('gray');
image(newImage);

%% 7 - Edge Detection

newImage = cast(newImage, 'double');
del2newImage = uint8(del2(newImage));


threshNewIm = del2newImage > 9;

%using del2() looks pretty bad, picks up a lot of pixels that aren't edges

figure(10);
colormap('gray');

edgeNewImage = uint8(edge(newImage,'Sobel'));

subplot(2,1,1)
imagesc(threshNewIm);title('del2(), thresh 9');

subplot(2,1,2)
imagesc(edgeNewImage);title('edge()');

% edge version looks much better, and is much easier because I don't have
% to deal with the thresholding guess and check


%% Part 2 - Debugging

% replace , with ; for new columns
A = [1 1 -2 1 3 -1;
     2 -1 1 2 1 -3;
     1 3 -3 -1 2 1;
     5 2 -1 -1 2 1;
     -3 -1 2 3 1 3;
     4 3 1 -6 -3 -2];

% transpose b
b = [4 20 -15 -3 16 -27]';

% solving operation is backslash
answer = round(A \ b);
correctAnswer = [1;-2;3;4;2;-1];

x = 0:0.01:10;
y1 = 2 * sin(5*x);
y2 = 1.5 * cos(2*x);
y3 = 0.5 * sin(2./x);

figure(11)
hold on
plot(x,y1, 'b', 'LineWidth',1); 
plot(x,y2, 'r', 'LineWidth',1)
plot(x,y3, 'g', 'LineWidth',1)
xlabel('x');
ylabel('y');
title('x vs. y');
legend('y1','y2','y3');

hold off


%%
function [image] = myImagesc(B)
% imitatezs imagesc() to work for grayscale of image
    oldMin = 0;
    oldMax = 170;
    minRange = 0;
    maxRange = 255;
    image = zeros(721,721);
    
    image(:,:) = (B(:,:) - oldMin) .* (maxRange - minRange) ./ (oldMax - oldMin) + minRange;
%     0 to 170 -- 0 to 255 --  170 * 250/170
     
end

    
