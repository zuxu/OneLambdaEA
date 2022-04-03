import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
from numpy import genfromtxt
import math

repeat = 10
filename = ['fDynamicBinval_c0.950000.csv', 'fDynamicBinval_c0.980000.csv', 'fDynamicBinval_c1.000000.csv', 'fDynamicBinval_c1.020000.csv', 'fDynamicBinval_c1.050000.csv']
label = ['$c=0.95$', '$c=0.98$', '$c=1.00$', '$c=1.02$', '$c=1.05$'] 
S = 15
s = [1.25, 1.5, 1.75, 2, 2.5, 3, 3.5, 4, 4.75, 5.5, 6.25, 7, 8, 9, 10]
n = 1000

def read(filename, S):
    data = genfromtxt(filename, delimiter=',') 
    print(data.shape)
    res = np.zeros((repeat, S, 2))
    for i in range(data.shape[0]):
        res[int(data[i,0]), int(data[i,1]), 0] = int(data[i,2])
        res[int(data[i,0]), int(data[i,1]), 1] = int(data[i,3])
    return res

data = []
for file in filename:
    data.append(read(file, S))

for i in range(len(filename)):
    mean = np.mean(data[i], axis=0)
    std = np.std(data[i], axis=0)
    plt.errorbar(s, mean[:, 0]/500/n, yerr=std[:, 0]/500/2/n, label=label[i])
plt.legend()
plt.xlabel('$F$')
ax = plt.gca()
ax.set_xscale('log')
ax.xaxis.set_major_formatter(mticker.StrMethodFormatter('{x:.0f}'))
ax.xaxis.set_minor_formatter(mticker.ScalarFormatter())

plt.ylabel('normalized number of generations')
plt.savefig('fDynamic.png')
plt.clf()


# plot for each function, outdated
#def draw(filename, n, low, high, S, figname):
#    res = read(filename, S)
#    mean = np.mean(res, axis=0)
#    std = np.std(res, axis=0)
#    x = []
#    for i in range(S):
#        x.append(1+low*math.pow(high/low, 1.0*i/(S-1)))
#    x = np.array(x)
#    
#    plt.errorbar(x, mean[:, 0]/n/500, yerr=std[:, 0]/n/500, label='n='+str(n))
#    plt.xscale('log')
#    
#    plt.legend()
#    plt.xlabel('F')
#    plt.ylabel('normalized number of generations')
#    plt.savefig(figname + '.png')
#    plt.clf()

#draw('fDynamicBinval.csv', 1000, 0.01, 9, 20, 'dynamicBinval_s1.8')
#draw('fBinval.csv', 1000, 0.01, 9, 20, 'binval_s2.2')
#draw('fBinary.csv', 1000, 0.01, 9, 20, 'binary_s2.8')
#draw('fOnemax.csv', 1000, 0.01, 9, 20, 'onemax_s3.2')
#draw('fHotopic.csv', 1000, 0.01, 9, 20, 'hotpic_s2.8')