import matplotlib.pyplot as plt
import numpy as np

for i in range(500):
    ds = np.loadtxt('out{:03d}.dat'.format(i))
    plt.plot(ds[:,0], ds[:,1])
    plt.xlim(0,1)
    plt.ylim(0,1)
    plt.savefig('img{:03d}.png'.format(i))
    plt.clf()
