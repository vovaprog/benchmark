import sys
import os
import re

import numpy as np

import matplotlib
if len(sys.argv) > 2:
    # next line is needed to generate plot files on a system without X windows
    matplotlib.use('Agg')

import matplotlib.pyplot as plt


plotCounter = 0
lineStyles = ["-", "--", "-.", ":"]
lineStyleIndex = 0

barIndex = 0

values = np.empty([1, 0])
names = []


def plotFile(fileName, prefix):
    global plotCounter
    global lineStyleIndex
    global barIndex
    global values

    x = np.loadtxt(fileName, dtype='int', delimiter='|')

    m = re.search("/([^/.]+).txt$", fileName)

    values = np.append(values, x)
    names.append(prefix + " " + m.group(1))


def plotFolder(folderName, prefix):
    files = []

    prefixSep = ""
    if len(prefix) > 0:
        prefixSep = " "

    for f in os.listdir(folderName):
        if os.path.isfile(os.path.join(folderName, f)):
            files.append(f)
        else:
            plotFolder(os.path.join(folderName, f), prefix + prefixSep + f)

    files = sorted(files, reverse=True)

    for f in files:
        plotFile(os.path.join(folderName, f), prefix)


folderName = "./build_release/plots"

if len(sys.argv) > 1:
    folderName = sys.argv[1]

plotFolder(folderName, "")


indexes = range(values.size)

plt.barh(indexes, values, align='center')
plt.yticks(indexes, names)


# plt.legend(fontsize=12)

# remove plot border
plt.figure(1).tight_layout(pad=0)

if len(sys.argv) > 2:
    plt.savefig(sys.argv[2])
else:
    plt.show()
