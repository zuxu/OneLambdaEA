import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt

N = [96, 1000, 10000]
n = 10000
s = 0.2
S = 25
repeat = 10
files = ['onemax.csv', 'hotopic.csv', 'binary.csv', 'binval.csv']
funcname = ['OneMax', 'Hot Topic', 'Binary', 'Binary Value']

def read(filename, N, S):
    data = genfromtxt(filename, delimiter=',') 
    print(data.shape)
    res = np.zeros((repeat, len(N), S, 2))
    for i in range(data.shape[0]):
        res[int(data[i,0]), int(data[i,1]), int(data[i,2])-1, 0] = int(data[i,3])
        res[int(data[i,0]), int(data[i,1]), int(data[i,2])-1, 1] = int(data[i,4])
    return res

def draw(filename, N, s, S, figname):
	
    res = read(filename, N, S)
    mean = np.mean(res, axis=0)
    std = np.std(res, axis=0)
    
    x = np.arange(1, S+1, 1) * s
    for i in range(len(N)):
        plt.errorbar(x, mean[i, :, 0]/N[i]/500, yerr=std[i, :, 0]/N[i]/500, label='n='+str(N[i]))
    plt.legend()
    plt.xlabel('$s$')
    plt.ylabel('normalized number of generations')
    #plt.title(figname)
    plt.savefig(figname + '.png')
    plt.clf()
    
# plot for each function
draw('binval.csv', N, s, S, 'binval')
draw('binary.csv', N, s, S, 'binary')
draw('hotopic.csv', N, s, S, 'hotopic')
draw('onemax.csv', N, s, S, 'onemax')

# all together
data = []
for file in files:
    data.append(read(file, N, S))
data.append(read('dynamicBinval.csv', [n], 15))
x = np.arange(1, S+1, 1) * s

for i in range(len(files)):
    mean = np.mean(data[i], axis=0)
    std = np.std(data[i], axis=0)
    plt.errorbar(x, mean[2, :, 0]/500/n, yerr=std[2, :, 0]/500/2/n, label=funcname[i])
mean = np.mean(data[4], axis=0)
std = np.std(data[4], axis=0)
plt.errorbar(x[0:15], mean[0, 0:15, 0]/500/n, yerr=std[0, 0:15, 0]/500/2/n, label='Dynamic Binval')
plt.legend()
plt.xlabel('$s$')
plt.ylabel('normalized number of generations')
plt.savefig('threshold.png')
plt.clf()