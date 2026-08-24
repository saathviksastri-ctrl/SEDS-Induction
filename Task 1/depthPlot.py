#Name - Saathvik Sastri    ID - F2026B3PS0499H

#importing libraries
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

#reading the data from provided csv file
data  = pd.read_csv('Depth Data.csv')

#setting up the graph to be plotted in real time
fig, ax = plt.subplots()
ax.set_xlabel('Point')
ax.set_ylabel('Depth (m)')
ax.set_title('Depth Data')

line = ax.plot([], [])[0]

displayWindow = 300 #how many values to display at once to avoid the graph from being too cluttered
#currently set to 300 to display the whole graph at the end, but can be adjusted to smaller value if needed

def update(frame):
    #sampling data up to the current frame to mimic receiving real time data
    curDat = data.iloc[:frame+1].copy()

    #converting to numeric and handling missing values through interpolation
    curDat['Depth (m)'] = pd.to_numeric(curDat['Depth (m)'], errors='coerce')
    curDat['Depth (m)'] = curDat['Depth (m)'].interpolate()

    #smoothing the data using a rolling median to handle erratic/corrupted readings
    curDat["smooth"] = curDat["Depth (m)"].rolling(window=3).median()
    curDat["smooth"] = curDat["smooth"].fillna(curDat["Depth (m)"])

    #updating the line data for the animation, resizing it to hold all the data points
    line.set_data(curDat['Point'], curDat['smooth'])
    ax.set_xlim(max(1, curDat['Point'].max() - displayWindow), max(2, curDat['Point'].max()))
    ax.set_ylim(curDat['smooth'].min() - 10, curDat['smooth'].max() + 10)
    return (line,)

#animating the function, setting interval between frames to 1 second
ani = animation.FuncAnimation(fig, update, frames=len(data), interval=1000, repeat = False)

plt.show()
