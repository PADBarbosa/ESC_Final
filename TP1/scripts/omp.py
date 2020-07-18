import sys
import re
import matplotlib.pyplot as plt; plt.rcdefaults()
import numpy as np
import matplotlib.pyplot as plt


#info for samples with n threads
class NThreads:


    def __init__(self, number, name):
        self.number = number
        self.name = name
        self.times = []
        self.times_avg = 0
        self.mops = []
        self.mops_avg = 0




def plot_results(plotName, attribute, all_results):

    maximumY = 0
    for results in all_results:
        x = list(map(lambda x : x.number, results))
        y = list(map(lambda x : getattr(x, attribute), results))
        maximumY = max(maximumY, max(y))
        plt.plot(x, y, '-o', label=results[0].name)

    plt.xlabel('Threads')
    plt.ylabel('Tempo(s)')
    plt.legend()
    plt.title(plotName)
    plt.yticks(np.arange(0, maximumY + 10, 10))
    plt.grid()
    plt.savefig("../Imagens/"  + plotName + ".png")
    plt.show()





plotName = sys.argv[1]
print(plotName)


files = sys.argv[2:]




all_results = []

for file in files:



    print(file)

    data = open(file, 'r').read()


    samples = re.split(r"([0-9]+) Threads", data)[1:]

    file_results = []

    name = re.search("Nome:\"([a-zA-Z 0-9]*)\"", data).group(1)

    for sample in samples:

        if re.fullmatch(r"[0-9]+", sample):
            numberOfThreads = sample
            print(numberOfThreads)
            nt = NThreads(numberOfThreads, name)
        else:
            nt.times = list(map(lambda x : float(x), re.findall(r"Time in seconds = +([0-9]+\.[0-9]+)", sample)))
            print(nt.times)
            nt.times_avg = np.mean(nt.times)

            nt.mops = list(map(lambda x : float(x), re.findall(r"Mop/s total     =  +([0-9]+\.[0-9]+)", sample)))
            print(nt.mops)
            nt.mops_avg = np.mean(nt.mops)


            file_results.append(nt)
    all_results.append(file_results)

plot_results(plotName, 'times_avg', all_results)
