import fileinput
import sys
import re
import matplotlib.pyplot as plt
import numpy as np



def plot_bar_graph(title, attribute, tickLabel, values):

    
    y_pos = np.arange(len(tickLabel))

    #plt.xticks(fontsize='x-small')

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
        self.total = values[0]
        self.used = values[1]
        self.free = values[2]
        self.shared = values[3]
        self.buffers = values[4]
        self.cached = values[5]


    def __str__(self):
        return str(self.used)

    def __repr__(self):
        return str(self.used)

def average(l, attribute):
    total = sum(getattr(c, attribute) for c in l)
    return(total/len(l))

def maximum(l, attribute):
    total = max(getattr(c, attribute) for c in l)
    return(total)

files = sys.argv[2:]
values = []
tickLabel = []
for file in files:
    fileContent = fileinput.input(file)
    print(file)
    samples = []


    for line in fileContent:
        if re.search(r"Mem: +[0-9]+", line):
            l = re.split(r" +", line)
            l = list(map(lambda x : int(x), l[1:]))
            sample = Sample(l)
            samples.append(sample)
    print(samples)
    samples = samples[1:]
    tickLabel.append(re.search(r"free_([a-z]{2}_[A-D])\.txt", file).group(1)) #sp bt lu
    values.append(maximum(samples, "used"))
    print(maximum(samples, "used"))


plot_bar_graph(sys.argv[1], "used", tickLabel, values)