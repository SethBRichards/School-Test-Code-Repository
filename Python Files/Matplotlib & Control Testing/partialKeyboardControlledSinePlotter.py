from __future__ import print_function
import math as m
import matplotlib.pyplot as plt
from inputs import get_key

switchBool = True
x = 0

# Keyboard Controlled Limited Sine Plotter
#
# This program allows the client to use arrow keys to
# control the position of a circle on a plot over discrete time.
#
# The purpose of this project was to learn about how to code in python,
# use matplotlib, take keyboard inputs, and work with discrete time control
#
# This code was create early on in my career, and thus is commented poorly


def main():
    global switchBool
    global x
    z =0
    a = 0
    c = 0
    d = 0

    plt.axis([0, 100, 0, 50])
    i = 0
    # for i in range(50):
    while i < 1000:
        i = i + 1
        events = get_key()
        if events:
            x = int(events[0].state)
            switchBool = not switchBool
            if switchBool:
                d = x


                if x == 72:
                    a = 1
                elif x == 80:
                    a = -1

                y = m.sin(i/10)
                z = d
                b = (c + a)*y
                c = b
                print("x  ",x)
                print("b"  ,b)
                plt.scatter(i/2, b)
                plt.pause(0.001)
                print(i)
                plt.copper()
                plt.autoscale(enable=True, axis='both', tight=False)
    plt.show()



if __name__ == "__main__":
    main()

