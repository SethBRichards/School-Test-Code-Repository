

from __future__ import print_function
import matplotlib.pyplot as plt
from inputs import get_key


# Keyboard Real Time Controller Testing
#
# This program takes real time keyboard inputs from the arrow keys,
# and uses them in a Matplotlib plot to controll a red circle object.
# I attempted to give the object an accelaration vector to simulate natural
# physics, but the whole program is a little buggy.
#
# The purpose of this project was to learn to program in Python,
# matplotlib plots, and keyboard inputs in coding projects. This is early
# on in m coding career, so my commenting is not fantastic

switchBool = True
x = 0


def main():
    global switchBool
    global x
    # initializing variables
    z = 0
    a = 0
    c = 0
    d = 0
    e = 0
    h = 0
    xp = 0
    yp = 0

    xv = 0 # x velocity
    yv = 0 # y velocity

    incrementVar = 0

    plt.axis([0, 100, 0, 100])
    i = 0
    # for i in range(50): <-- early attempt at a for loop
    while i < 1000:
        i = i + 1
        events = get_key()
        if events:
            x = int(events[0].state)
            switchBool = not switchBool

            # attempt at creating an acceleration vector
            if switchBool:
                d = x

                # values thrown from arrow keys
                if x == 72:
                    xv = 1
                    yv = 0
                    if x == h:

                        xp = xp + (2*xv)
                    else:
                        xp = xp + xv

                elif x == 80:
                    xv = -1
                    yv = 0
                    if x == h:
                        xp = xp + (2*xv)
                    else:
                        xp = xp + xv


                elif x == 77:

                    yv = 1
                    xv = 0

                    xp = xp + xv
                    yp = yp + yv

                elif x == 75:
                    yv = -1
                    xv = 0

                    xp = xp + xv
                    yp = yp + yv

                # creating red circule object twice, to maintain
                # shape while moving by creating an empty circle
                # overtop the previous, twice
                plt.plot(yp, xp, 'ro',markersize=15)
                plt.plot(c, e, 'wo', markersize=16)
                plt.plot(yp, xp, 'ro', markersize=15)

                c = yp
                e = xp

                plt.pause(0.001)
                #print(i)

                h = x

    plt.show()

# run function
if __name__ == "__main__":
    main()
