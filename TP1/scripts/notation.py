import sys
import re
import matplotlib.pyplot as plt; plt.rcdefaults()
import numpy as np
import matplotlib.pyplot as plt

file = sys.argv[1]


data = open('perf.txt', 'r').read()

values = list(map(lambda x : np.format_float_scientific(int(x),precision=2), re.findall(r"[0-9][0-9][0-9][0-9][0-9]+", data)))
#print(times)
for v in values:
	print(v)

