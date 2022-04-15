import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt

def smooth(a, size, window):
    b = a.copy()
    
    for i in range(0, window+1):
        b[i] = np.mean(a[0:i+window+1])
    ind = window + 1
    while ind+window<size:
        b[ind]= np.mean(a[ind-window:ind+window+1])
        ind += 1
    
    while(ind < size):
        b[ind] = np.mean(a[ind-window:size])
        ind += 1
    return b

def read(filename, n, S, repeat):
    
    data = genfromtxt(filename, delimiter=',') 
    print(data.shape)
    res = np.zeros((repeat, len(S), 4, n)) # repeat, number of s, [generation, lambda, drift, evaluation], n
    cnt = 0
    for i in range(repeat):
        for j in range(len(S)):
            for k in range(n):
                res[i, j, 0, k] = data[cnt, 0]
                res[i, j, 1, k] = data[cnt, 1]
                res[i, j, 2, k] = data[cnt, 2]
                res[i, j, 3, k] = data[cnt, 1]
                cnt = cnt + 1
    
    mix = np.sum(res, axis=0)
    sum_gen = np.sum(mix[:, 0, :], axis=1)
    mix[:, 1, :] /= np.where(mix[:, 0, :]<1e-10, 1, mix[:, 0, :])
    mix[:, 2, :] /= np.where(mix[:, 0, :]<1e-10, 1, mix[:, 0, :])
    mix[:, 0, :] /= np.where(sum_gen[:, np.newaxis]<1e-10, 1, sum_gen[:, np.newaxis])
    sum_eva = np.sum(mix[:, 3, :], axis=1)

    mix[:, 3, :] /= np.where(sum_eva[:, np.newaxis]<1e-10, 1, sum_eva[:, np.newaxis])    
    return mix

def draw(mix, n, S, funcname):
    for i in range(len(S)):
        fig, ax = plt.subplots(2, 2, sharex=True, figsize=(12, 8))

        x = np.arange(0, n, 1)
        for j in range(len(data)):
            index = (mix[j][i, 0, :] > 0)
            ax[0, 0].errorbar(x[index], mix[j][i, 0, index], yerr=0, label=str(funcname[j]))
        ax[0, 0].legend()
        ax[0, 0].set_xlabel('onemax value')
        ax[0, 0].set_ylabel('normalized number of generations')
        #plt.savefig(funcname+'_gen.png')
        #plt.clf()

        for j in range(len(data)):
            index = (mix[j][i, 3, :] > 0)
            ax[0, 1].errorbar(x[index], mix[j][i, 3, index], yerr=0, label=str(funcname[j]))
        ax[0, 1].legend()
        ax[0, 1].set_xlabel('onemax value')
        ax[0, 1].set_ylabel('normalized number of evaluations')

        for j in range(len(data)):
            index = (mix[j][i, 0, :] > 0)
            ax[1, 0].errorbar(x[index], mix[j][i, 1, index], yerr=0, label=str(funcname[j]))
        ax[1, 0].legend()
        ax[1, 0].set_xlabel('onemax value')
        ax[1, 0].set_yscale('log')
        ax[1, 0].set_ylabel('mean of $\lambda$')
        #plt.savefig(funcname+'_lbd.png')
        #plt.clf()

        for j in range(len(data)):
            size = np.sum(mix[j][i, 0, :] > 0)
            ax[1, 1].errorbar(x[:size], smooth(mix[j][i, 2, :], size, 7)[:size], yerr=0, label=str(funcname[j]))

        ax[1, 1].axhline(0, alpha=0.7, linestyle='--', color='grey')
        ax[1, 1].legend()
        ax[1, 1].set_xlabel('onemax value')
        ax[1, 1].set_ylabel('smoothed mean of drift')
        plt.savefig('average_'+str(S[i])+'.png')
    
data = []
filename = ['onemax_details.csv', 'binary_details.csv', 'hotopic_details.csv', 'binval_details.csv', 'dynamicBinval_details.csv']
n = 1000
repeat = 100
S = [1, 2, 3, 4]

for file in filename:
    data.append(read(file, n, S, repeat))
    
draw(data, n, S, ['Onemax', 'Binary', 'Hot Topic', 'BinVal', 'Dynamic BinVal'])
    
