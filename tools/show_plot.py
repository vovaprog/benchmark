import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join
import re
import sys
import os

plotCounter = 0
lineStyles = ["-", "--", "-.", ":"]
lineStyleIndex = 0


def plotFile(fileName, prefix):
    global plotCounter
    global lineStyleIndex

    x, y = np.loadtxt(fileName, dtype='int', delimiter='|', unpack=True)

    m = re.search("/([^/.]+).txt$", fileName)

    plt.plot(x, y, label=prefix + " " + m.group(1),
             linewidth=2, linestyle=lineStyles[lineStyleIndex])

    plotCounter += 1
    if plotCounter >= 7:
        plotCounter = 0
        lineStyleIndex += 1
        if lineStyleIndex >= len(lineStyles):
            lineStyleIndex = 0


def plotFolder(folderName, prefix):
    files = []

    prefixSep = ""
    if len(prefix) > 0:
        prefixSep = " "
    
    for f in listdir(folderName):
        if isfile(join(folderName, f)):
            files.append(f)
        else:
            plotFolder(join(folderName, f), prefix + prefixSep + f)

    files = sorted(files)
    
    for f in files:
        plotFile(join(folderName, f), prefix)
        

folderName = "./build_release/plots"

if len(sys.argv) > 1:
    folderName = sys.argv[1]

plotFolder(folderName, "")

plt.legend(fontsize=8)

plt.xlabel("items")
plt.ylabel("microseconds")

# remove plot border
#plt.figure(1).tight_layout(pad=0)


if len(sys.argv) > 2:
    width = 700.0
    height = width * 0.6
    dpi = plt.figure(1).get_dpi()
    plt.figure(1).set_size_inches(width/float(dpi), height/float(dpi))

    #os.makedirs(os.path.dirname(sys.argv[2]))
    plt.savefig(sys.argv[2])     
else:
    plt.show()

