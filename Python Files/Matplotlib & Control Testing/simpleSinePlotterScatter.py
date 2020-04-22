import numpy as np
import math as m
import matplotlib.pyplot as plt

# simple test for scatterplot overtime with a sine wave, infinitly

plt.axis([0, 10, -2, 2])
i=0
#for i in range(50):
while i < 1000:
    i = i + 1

  #  y = np.random.random()
    y=m.sin(i)
    plt.scatter(i, y)
    plt.pause(.1)
print(i)
plt.copper()
plt.autoscale(enable=True,axis='x', tight=False)
plt.show()

print(i)
c = 5

