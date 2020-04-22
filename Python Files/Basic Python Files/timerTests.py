import time

class basicTimer:

    runBoolean = False
    initialTime = int(time.time())
    timeVariable = 0
    timeHolder = -1


    def runTimer(self):
        while self.runBoolean == True:

            self.timeVariable = int(time.time() - self.initialTime)
            if self.timeVariable > self.timeHolder:
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



#bt = basicTimer()

#bt.start()
print("        f")
#print ("        " , bt.timeVariable)
#while x == 0:
   # print (bt.timerVariable)
   # print("f")
