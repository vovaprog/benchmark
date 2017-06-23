import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join
import re
import sys

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
    
    for f in listdir(folderName):
        if isfile(join(folderName, f)):
            files.append(f)
        else:
            plotFolder(join(folderName, f), prefix + prefixSep + f)

    files = sorted(files, reverse=True)
    
    for f in files:
        plotFile(join(folderName, f), prefix)
        

folderName = "./build_release/plots"

if len(sys.argv) > 1:
    folderName = sys.argv[1]

plotFolder(folderName, "")


indexes = range(values.size)

plt.barh(indexes, values, align='center')
plt.yticks(indexes, names)


#plt.legend(fontsize=12)

# remove plot border
plt.figure(1).tight_layout(pad=0)

if len(sys.argv) > 1:
    plt.savefig(sys.argv[2])
else:
    plt.show()

