import time

from timerTests import basicTimer
from threading import Thread


def printF():
    f = 2
    while f == 2:
        print ("     -----   ", tim.timeVariable," ---")

def printA():
    f = 2
    while f == 2:
        print ("aaaaaa")



tim = basicTimer()

if __name__ == '__main__':
    Thread(target = tim.start).start()
    Thread(target = tim.stop).start()



#print ("f")
#tim.start()
#print ("f")

#if __name__ == '__main__':
 #   Thread(target = func1).start()
#    Thread(target = func2).start()


