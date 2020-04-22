
import time
import matplotlib.pyplot as plt

class basicTimer:

    runBoolean = False
    initialTime = int(time.time())
    timeVariable = 0
    timeHolder = -1


    def runTimer(self):
        while basicTimer.runBoolean == True:

            self.timeVariable = int(time.time() - self.initialTime)
            if self.timeVariable > self.timeHolder:
                plt.pause(.1)
                print(self.timeVariable)
                self.timeHolder = self.timeVariable
                #print ("tV", self.timeVariable , "      tH" , self.timeHolder)

    def start(self):

        basicTimer.initialTime = int(time.time())
        basicTimer.timeVariable = 0
        basicTimer.timeHolder = -1
        basicTimer.runBoolean = True

            #     timeHolder = timeVariable

    def resetTimer(self):
        basicTimer.initialTime = time.time()

    def stopTimer(self):
        basicTimer.runBoolean = False


    def __init__(self):
     if basicTimer.runBoolean == True:
         print("uerhgiuerhgieurhg")



def func1():
    print("fffff")

def func3():
    print("aaaaaaaaa")




bt = basicTimer()

for afunc in (func1, bt.start(),bt.runTimer(), func3):
    afunc()


print("        f")
#print ("        " , bt.timeVariable)
#while x == 0:
   # print (bt.timerVariable)
   # print("f")
