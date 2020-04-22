
from __future__ import print_function

from inputs import get_key


"""Simple example showing how to get keyboard events."""


switchBool = True
x = 0
def main():
    """Just print out some event infomation when keys are pressed."""


    global switchBool
    global x


    while True:

        events = get_key()
        if events:
            x = int(events[0].state)
            switchBool = not switchBool
            if switchBool:
                print(x)


if __name__ == "__main__":
    main()

