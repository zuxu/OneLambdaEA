import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt
from mpl_toolkits.axes_grid1.inset_locator import (inset_axes, InsetPosition,
                                                  mark_inset, zoomed_inset_axes)

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
    style = ['dotted', 'dashed', 'dashdot', 'solid', 'solid']
    legend_size = 13.5
    font_size = 14.5
    for i in range(len(S)):
        fig, ax = plt.subplots(2, 2, sharex=False, figsize=(12, 8))

        x = np.arange(0, n, 1)
        for j in range(len(data)):
            index = (mix[j][i, 0, :] > 0)
            ax[1, 0].plot(x[index], mix[j][i, 0, index], label=str(funcname[j]), linestyle=style[j])
        ax[1, 0].legend(fontsize=legend_size)
        ax[1, 0].set_xlabel('onemax value', fontsize=font_size)
        ax[1, 0].set_ylabel('normalized number of generations', fontsize=font_size)
        #plt.savefig(funcname+'_gen.png')
        #plt.clf()
        
        for j in range(len(data)):
            index = (mix[j][i, 3, :] > 0)
            ax[1, 1].plot(x[index], mix[j][i, 3, index], label=str(funcname[j]), linestyle=style[j])
        ax[1, 1].legend(fontsize=legend_size)
        ax[1, 1].set_xlabel('onemax value', fontsize=font_size)
        ax[1, 1].set_ylabel('normalized number of evaluations', fontsize=font_size)
        
        
        if S[i]==2 or S[i]==3:
            axinset = zoomed_inset_axes(ax[0, 0], zoom=2, loc=2)
            if S[i]==2:
                x1, x2, y1, y2 = 600, 900, 1, 10
            else:
                x1, x2, y1, y2 = 500, 900, 1, 4
            axinset.set_xlim(x1, x2)
            axinset.set_ylim(y1, y2)
            #axinset.tick_params(labelleft=False, labelbottom=False)
            axinset.set_yscale('log')
            axinset.xaxis.set_visible(False)
            axinset.yaxis.set_visible(False)
            mark_inset(ax[0, 0], axinset, loc1=1, loc2=3, fc='none', ec='0.5')
        
        #colors = ['blue', 'orange', 'green', 'red', 'purple']
        colors = [u'#1f77b4', u'#ff7f0e', u'#2ca02c', u'#d62728', u'#9467bd']
        for j in range(len(data)):
            size = np.sum(mix[j][i, 0, :] > 0)
            y = smooth(mix[j][i, 1, :], size, 7)[:size]
            ax[0, 0].plot(x[:size], y, label=str(funcname[j]), linestyle=style[j])
            if S[i]==2 or S[i]==3: 
                if x2 < size:
                    size = x2
                axinset.plot(x[x1:size], y[x1:size], label=str(funcname[j]), color=colors[j], linestyle=style[j])
            #subset = np.arange(0, x.size, 10)
            #subx = x[subset]
            #suby = mix[j][i, 1, subset]
            #subind = (mix[j][i, 0, subset] > 0)
            #ax[0, 0].errorbar(subx[subind], np.log(suby[subind]), yerr=0, label=str(funcname[j]))
        #ax[0, 0].legend()
        ax[0, 0].set_yscale('log')
        #ax[0, 0].set_xlabel('onemax value')
        ax[0, 0].set_ylabel('smoothed mean of $\lambda$', fontsize=font_size)
        #plt.savefig(funcname+'_lbd.png')
        #plt.clf()
        
        

        for j in range(len(data)):
            size = np.sum(mix[j][i, 0, :] > 0)
            ax[0, 1].plot(x[:size], smooth(mix[j][i, 2, :], size, 7)[:size], label=str(funcname[j]), linestyle=style[j])

        ax[0, 1].axhline(0, alpha=0.7, linestyle='--', color='grey')
        ax[0, 1].legend(fontsize=legend_size)
        #ax[0, 1].set_xlabel('onemax value')
        ax[0, 1].set_ylabel('smoothed mean of drift', fontsize=font_size)
        plt.savefig('average_'+str(S[i])+'.png')
        
    
data = []
filename = ['onemax_details.csv', 'binary_details.csv', 'hotopic_details.csv', 'binval_details.csv', 'dynamicBinval_details.csv']
n = 1000
repeat = 100
S = [1, 2, 3, 4]

for file in filename:
    data.append(read(file, n, S, repeat))

draw(data, n, S, ['Onemax', 'Binary', 'Hot Topic', 'BinVal', 'Dynamic BinVal'])


