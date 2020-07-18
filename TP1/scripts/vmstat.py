import fileinput
import sys
import re
import matplotlib.pyplot as plt
import numpy as np



def plot_bar_graph(title, attribute, tickLabel, values):

    
    y_pos = np.arange(len(tickLabel))

    plt.xticks(fontsize='x-small')

    plt.bar(y_pos, values, align='center', alpha=0.5, tick_label=tickLabel)

    plt.title(title)
    plt.xlabel('')
    plt.ylabel('Megabytes')
    plt.savefig("../Imagens/"  + title + ".png")

    plt.show()


def plot_attribute(l, attribute):
    x = list(range(0, len(l)))
    y = list(map(lambda x : getattr(x, attribute), l))

    plt.plot(x, y, color=(0,0,1), label=attribute)
    plt.xlabel('')
    plt.ylabel('')
    plt.legend()
    plt.show()


class Sample:

    def __init__(self, values):
        #procs
        self.r = values[0]
        self.b = values[1]

        #memory
        self.swpd = values[2]
        self.free = values[3]
        self.buff = values[4]
        self.cache = values[5]

        #swap
        self.si = values[6]
        self.so = values[7]

        #io
        self.bi = values[8]
        self.bo = values[9]

        #system
        self.inn = values[10]
        self.cs = values[11]
        self.us = values[12]

        #cpu
        self.sy = values[13]
        self.id = values[14]
        self.wa = values[15]
        self.st = values[16]


    def __str__(self):
        return str(self.free)

    def __repr__(self):
        return str(self.free)

def average(l, attribute):
    total = sum(getattr(c, attribute) for c in l)
    return(total/len(l))




files = sys.argv[2:]
values = []
tickLabel = []
for file in files:
    
    fileContent = fileinput.input(file)
    samples = []
    
    for line in fileContent:
        if re.search(r"[0-9]+", line):
            l = re.split(r" +", line)
            l = list(map(lambda x : int(x), l[1:]))
            sample = Sample(l)
            samples.append(sample)
    
    print(samples)
    samples = samples[1:]
    tickLabel.append(re.search(r"vmstat_([a-z]{2}_[A-D])\.txt", file).group(1)) #sp bt lu
    values.append(average(samples, "free"))
    print(average(samples, "free"))

plot_bar_graph(sys.argv[1], "free", tickLabel, values)
