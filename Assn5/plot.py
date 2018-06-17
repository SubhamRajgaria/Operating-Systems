#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

with open("instructions2.txt") as f:
    data = f.read()

data = data.split('\n')

x = [row.split(' ')[0] for row in data[0:10000]]
y = [row.split(' ')[1] for row in data[0:10000]]

fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("References ")    
ax1.set_xlabel('Time')
ax1.set_ylabel('Page No')

ax1.plot(x,y,'ro', label='The page reference')

leg = ax1.legend()

plt.show()
fig.savefig("tenk.png")