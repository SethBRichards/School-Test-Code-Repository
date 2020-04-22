a, e = 1, -1
b = d = 2.2
c = 'c'
m = 20


def main(m):
    print(float(m))

def doStuff(x,y):
    for i in range(y):
        #print(i)
        x = x + x
        print(x)
    return x


g = doStuff(1,5)

def myFunc(ab, cd):

    print(ab,cd)

#myFunc(cd =2, ab =4)

def testA(*args):
    for x in args:
        x = x + 1
        print(x)

#testA(2,3,5,8,-1,.2)

#abc = 0
"""ac = 0
def main2():
    for x in range(5,10):
       ac = abc + x
       abc = ac
       print(abc)
    return abc
abcde = 0.0
abcde = main2()
print(abcde)"""
