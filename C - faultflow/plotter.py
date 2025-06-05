import json
import csv
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import pandas as pd
import sys
import itertools
import matplotlib.pyplot as plt
from matplotlib import colormaps
import itertools
# --------------------------------------------------

csv.field_size_limit(sys.maxsize) # if it's not working put 922337200

plt.rcParams.update({
    "font.size": 14,        # base size for all text
    "axes.titlesize": 16,   # title
    "axes.labelsize": 14,   # x- and y-axis labels
    "xtick.labelsize": 14,  # x-tick numbers
    "ytick.labelsize": 14,  # y-tick numbers
    "legend.fontsize": 12,
    "axes.grid"     : True
})


df = pd.read_csv("result-pyramis.csv", skiprows=2)
yvalues = df["Values: "]
num = yvalues.count()
ticks = np.arange(0, num, 1)

plt.clf()
plt.rc('grid', linestyle=":", color='gray')
plt.xlim(0.0, 2000.0)
plt.plot( ticks, yvalues, color='#FF9999', label='AZ_Failure')
plt.xlabel('time (h)')
plt.grid()
plt.legend(loc = 'upper left')
plt.savefig('caseStudy_cdf_pyramis.pdf')

plt.rcParams.update({'font.size': 12})

plt.figure(figsize=(10,5))

with open("birnbaum.csv") as file:
    colors = ['#e60000', '#0000e6', '#00cc00', '#ffbf00', '#e60000', '#0000e6', '#00cc00', '#ffbf00', '#e60000', '#0000e6', '#00cc00', '#ffbf00']
    styles = ['solid', 'solid', 'solid', 'solid', 'dashed', 'dashed', 'dashed', 'dashed', 'dotted', 'dotted', 'dotted', 'dotted']
    heading = next(file)
    heading = next(file)
    heading = next(file)
    reader_obj = csv.reader(file, delimiter=":")
    plt.clf()
    count = 0
    for row in reader_obj:
        values = row[1][2:-1].split(',')
        yvalues = []
        label = row[0]
        for value in values:
            yvalues.append(float(value))
        num = len(yvalues)
        num = num*0.005
        ticks = np.arange(0, num, 0.005)
        plt.plot( ticks, yvalues, label=label, color=colors[count], linestyle=styles[count])
        count = count + 1
    plt.xlim(ticks[0], 2.5)
    handles, labels = plt.gca().get_legend_handles_labels()
    number = len(handles)
    order = sorted(range(0, number))
    plt.legend([handles[idx] for idx in order],[labels[idx] for idx in order])
    plt.xlabel('time ($10^6$ h)')
    plt.rc('grid', linestyle=":", color='gray')
    plt.grid()
    plt.tight_layout()
    plt.savefig("birnbaum.pdf")

plt.rcParams.update({'font.size': 16})

plt.figure(figsize=(10,5))

with open("fussellvesely.csv") as file:
    linestyles10 = ['solid', 'dashed','solid', 'dashed','solid', 'dashed','solid', 'dashed','solid', 'dashed' ]
    palette10 = ['#e60000','#ffbf00',
    '#0000e6','#e60000',
    '#ffbf00','#00cc00',
    '#00cc00','#0000e6',
    "#00D4FF", "#FF1493"]

    heading = next(file)
    heading = next(file)
    heading = next(file)

    reader_obj = csv.reader(file, delimiter=":")

    count = 0
    for row in reader_obj:
        if count >= 0:
            values = row[1][2:-1].split(',')
            yvalues = []
            label = row[0]
            for value in values:
                yvalues.append(float(value))
            num = len(yvalues)
            num = num*0.05
            ticks = np.arange(0, num, 0.05)
            plt.plot( ticks, yvalues, label=label,color=palette10[count],
            linestyle=linestyles10[count])
            count = count + 1
    plt.xlim(ticks[0], ticks[-1])
    handles, labels = plt.gca().get_legend_handles_labels()
    number = len(handles)
    order = sorted(range(0, number))
    plt.legend([handles[idx] for idx in order],[labels[idx] for idx in order], loc='center right')
    plt.xlabel('time ($10^6$ h)')
    plt.rc('grid', linestyle=":", color='gray')
    plt.grid()
    plt.tight_layout()
    plt.savefig("fusselvesely.pdf")
