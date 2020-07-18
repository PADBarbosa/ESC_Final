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
    plt.ylabel('%')
    plt.savefig("../Imagens/"  + title + ".png")

    plt.show()


def plot_attribute(title, ls, attribute):

    for l in ls:
        x = list(range(0, len(l)))
        y = list(map(lambda x : getattr(x, attribute), l))

        plt.plot(x, y, label=l[0].file)


    #plt.xlim(0,50)
    plt.ylim(bottom=0)

    plt.title(title)
    plt.xlabel('Time(s)')
    plt.ylabel(attribute + "%")
    plt.legend()
    plt.savefig("../Imagens/"  + title + ".png")
    plt.show()


class Sample:

    def __init__(self, file, values):
        self.file = file
        self.rrqm = values[0]
        self.wrqm = values[1]
        self.r = values[2]
        self.w = values[3]
        self.rMB = values[4]
        self.wMB = values[5]
        self.avgrq = values[6]
        self.avgqu = values[7]
        self.await = values[8]
        self.svctm = values[9]
        self.util = values[10]


    def __str__(self):
        return str(self.util)

    def __repr__(self):
        return str(self.util)

def average(l, attribute):
    total = sum(getattr(c, attribute) for c in l)
    return(total/len(l))

files = sys.argv[2:]
values = []
tickLabel = []
samples_list = []
for file in files:
    
    fileContent = fileinput.input(file)
    samples = []
    
    for line in fileContent:
        if re.search(r"sda +[0-9]+\.[0-9]+", line):
            print(line)
            l = re.split(r" +", line)
            l = list(map(lambda x : float(x), l[1:]))
            sample = Sample(re.search(r"iostat_([a-z]{2}_[A-D])\.txt", file).group(1), l)
            samples.append(sample)
    
    print(samples)
    samples = samples[1:]
    samples_list.append(samples)
    tickLabel.append(re.search(r"iostat_([a-z]{2}_[A-D])\.txt", file).group(1)) #sp bt lu
    values.append(average(samples, "util"))
    print(average(samples, "util"))

#plot_bar_graph(sys.argv[1], "util", tickLabel, values)
plot_attribute(sys.argv[1], samples_list, "util")