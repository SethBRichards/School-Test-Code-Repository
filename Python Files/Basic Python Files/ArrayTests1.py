import random


list = [5,2,6]

list[0] = 1
i=6

list.insert(3,8)



while i < 5:
    print(list)
    i = i+1


#for x in list:
    #list.insert(x+1,x+1)
   # print(list)
jj = random.randint(5,8)
#print(jj)
a = 25
c = 0
list1 = [0]
while list1[c] < a:

    list1.insert(c+1, random.randint(0,26))
    #print(c)
    #print("str")
   # print(list1)
    c = c + 1

list1.pop()
#print(list1)
n=0
g=0
m=0
for g in list1:
    n = list1[m]+n
    m+=1
#print(n)
ar2 =[5]
ar3 =[66]
ar1 =[ar2,ar3],[ar3,ar2]
print(ar1[1][0])
