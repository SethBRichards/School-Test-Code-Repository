function [r,theta] = cartesian2polar(x,y)
%Cartesian2polar converts cartesian coordinates to polar coordinates
%   Math

    r = sqrt((x^2)+ (y^2));
    theta = atan(y/x);
end

