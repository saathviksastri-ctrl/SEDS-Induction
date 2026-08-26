# Task 1
I used the pandas library to retrieve data from the given csv file and store in a data frame. It was mentioned in the task to treat the data as if it were being measured in real time, so I sampled the data up to the current frame each loop to mimic real time sensor readings.

After sampling the data, I forcibly converted it to numeric and filled nans via interpolation.
There were still extreme readings causing sudden spikes and dips in the graphs, so I needed to further process the data to eliminate these extreme values. I used a rolling median function inbuilt into pandas to set each value in the data frame to be the median of previous values, which reduced the influence of extreme points. Median was used instead of mean because if mean were used, one or two extreme readings would corrupt all the readings around them.

The data is animated and plotted using matplotlib. The graph resizes dynamically to hold a set number of values. For the given problem, it shows up to 300 at a time, but the window can be modified.

# Task 2
Tinkercad file - https://www.tinkercad.com/things/6g4WgMHPjCb-seds-induction?sharecode=_mku68GEtB4H-tznsenrQVKaqxvVpCpIppiXahaEIls
