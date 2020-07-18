import fileinput
import sys
import re
import matplotlib.pyplot as plt; plt.rcdefaults()
import numpy as np
import matplotlib.pyplot as plt


class Sample:

    def __init__(self, values):
        #self.time = values[0]
        #self.cpu = values[1];
        self.usr = values[0]
        self.nice = values[1]
        self.sys = values[2]
        self.iowait = values[3]
        self.irq = values[4]
        self.soft = values[5]
        self.steal = values[6]
        self.guest = values[7]
        self.idle = values[8]

    def __str__(self):
        return str(self.usr)

    def __repr__(self):
        return str(self.usr)

class CPU:
    def __init__(self, number):
        self.number = number
        self.samples = []


    def __eq__(self, other):
        return self.number  == other.number

    def __repr__(self):
        return "CPU: "+ str(self.number) + ", Samples: " + str(self.samples)

    def average(self, attribute):
        total = sum(getattr(c, attribute) for c in self.samples)
        return(total/len(self.samples))




def plot_core_usage(title, attribute, toFilter=False):

    if toFilter:
        filteredList = list(filter(lambda x: x.average(attribute) > 1, cpus_list))
    else:
        filteredList = cpus_list

    objects = list(map(lambda x : x.number, filteredList))
    #print(objects)
    y_pos = np.arange(len(objects))

    performance = list(map(lambda x : x.average(attribute), filteredList))
    #print(performance)

    tick_label = (list(map(lambda x : x.number, filteredList)))

    plt.xticks(fontsize='x-small')

    plt.bar(y_pos, performance, align='center', alpha=0.5, tick_label=tick_label)

    plt.title(title)
    plt.xlabel('Core')
    plt.ylabel('Usage(%)')
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



d = {}

title = sys.argv[1] 

fileContent = fileinput.input(sys.argv[2])




number_of_cores = int(re.search(r"([0-9]+) CPU", fileContent[0]).group(1))

print(number_of_cores)

cpus_list = []

for i in range(0,number_of_cores):
    cpus_list.append(CPU(i))

cpus_list.append(CPU("all"))



for line in fileContent:
    if re.search(r"[0-9]+:[0-9]+:[0-9]+ +(all|[0-9]+)", line):
        l = re.split(r" +", line)
        cpu_number = l[1]
        l = list(map(lambda x : float(x), l[2:]))
        sample = Sample(l)
        #print(sample.cpu, sample.sys)
        if cpu_number == "all":
            cpus_list[number_of_cores].samples.append(sample)
        else:
            cpus_list[int(cpu_number)].samples.append(sample)

print(cpus_list[12].average("usr"))




plot_core_usage(title,"usr",toFilter=False)
#plot_attribute(cpus_list[12].samples, "idle")