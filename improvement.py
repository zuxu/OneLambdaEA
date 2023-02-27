import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt
from mpl_toolkits.axes_grid1.inset_locator import (inset_axes, InsetPosition,
                                                  mark_inset, zoomed_inset_axes)

n = 1000
step = 20

name = ['Onemax', 'Binary', 'Hot Topic', 'BinVal', 'Dynamic BinVal']
style = ['dotted', 'dashed', 'dashdot', 'solid', 'solid']
data = genfromtxt('improvement.csv', delimiter=',')
res = np.zeros((len(name), n//step-1))
for i in range(data.size-1):
    res[i%len(name), int(i/len(name))] = data[i]

ax = plt.gca()

axinset = zoomed_inset_axes(ax, zoom=3.5, loc=3)
x1, x2, y1, y2 = 500, 600, 0.2, 0.3
axinset.set_xlim(x1, x2)
axinset.set_ylim(y1, y2)
#axinset.tick_params(labelleft=False, labelbottom=False)
#axinset.set_yscale('log')
axinset.xaxis.set_visible(False)
axinset.yaxis.set_visible(False)
mark_inset(ax, axinset, loc1=1, loc2=4, fc='none', ec='0.5')
colors = [u'#1f77b4', u'#ff7f0e', u'#2ca02c', u'#d62728', u'#9467bd']
    
for i in range(len(name)):
    ax.plot(range(step, n, step), res[i, :], label=name[i], linestyle=style[i])
    axinset.plot(range(x1, x2+1, step), res[i, x1//step-1:x2//step], color=colors[i], linestyle=style[i])
ax.legend(fontsize=14)
#ax.tick_params(size=12)
#plt.yscale('log')
ax.set_xlabel('onemax value', fontsize=16)
ax.set_ylabel('$p_{imp}$', fontsize=16)
plt.savefig('improvement.png')
    


