import fileinput
import sys
import re
import matplotlib.pyplot as plt
import numpy as np



def plot_bar_graph(title, attribute, tickLabel, values):

    
    y_pos = np.arange(len(tickLabel))

    #plt.xticks(fontsize='x-small')
    plt.ticklabel_format(axis="y", style="sci", scilimits=(0,0))


    plt.bar(y_pos, values, align='center', alpha=0.5, tick_label=tickLabel)

    plt.title(title)
    plt.xlabel('')
    plt.ylabel(attribute)
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
        #self.iface = values[0]
        self.rxpck = values[0]
        self.txpck = values[1]
        self.rxkB = values[2]
        self.txkB = values[3]
        self.rxcmp = values[4]
        self.txcmp = values[5]
        self.rxmcst = values[6]
        


    def __str__(self):
        return str(self.txkB)

    def __repr__(self):
        return str(self.txkB)

def totalSum(l, attribute):
    total = sum(getattr(c, attribute) for c in l)
    return(int(total))

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
        if re.search(r"[0-9]{2}\:[0-9]{2}\:[0-9]{2} +[a-z]+[0-9]? +[0-9]+\.[0-9]+", line):
            l = re.split(r" +", line)
            l = list(map(lambda x : float(x), l[2:]))
            sample = Sample(l)
            samples.append(sample)
            
    print(samples)
    samples = samples[1:]
    tickLabel.append(re.search(r"sar_([a-z]{2}_[A-D])\.txt", file).group(1)) #sp bt lu
    values.append(totalSum(samples, "txkB"))
    print(totalSum(samples, "txkB"))

plot_bar_graph(sys.argv[1], "txkB", tickLabel, values)