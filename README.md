# Task 1
I used the pandas library to retrieve data from the given csv file and store in a data frame. It was mentioned in the task to treat the data as if it were being measured in real time, so I sampled the data up to the current frame each loop to mimic real time sensor readings.

After sampling the data, I forcibly converted it to numeric and filled nans via interpolation.
There were still extreme readings causing sudden spikes and dips in the graphs, so I needed to further process the data to eliminate these extreme values. I used a rolling median function inbuilt into pandas to set each value in the data frame to be the median of previous values, which reduced the influence of extreme points. Median was used instead of mean because if mean were used, one or two extreme readings would corrupt all the readings around them.

The data is animated and plotted using matplotlib. The graph resizes dynamically to hold a set number of values. For the given problem, it shows up to 300 at a time, but the window can be modified.



# Task 2
Tinkercad file - https://www.tinkercad.com/things/6g4WgMHPjCb-seds-induction?sharecode=_mku68GEtB4H-tznsenrQVKaqxvVpCpIppiXahaEIls

The system is a state machine with 5 states - open sea, storm, charybdis, anchor down and wrecked. Each loop, the state machine checks the state and runs several functions to evaluate whether the ship is in danger in the handleState() function. 

Storm - If the light reading is below half the total light, storm is detected. The LDR was experimentally returning values ranging from 54 at minimum light to 974 at maximum, so the readings were mapped to a 0-100 range. If the light level drops below 50, storm is declared. A pulse is sent every half second to an LED, which blinks when the state is storm. Note - In tinkercad, the light sensor starts out with a default reading of 0, so the ship will by default start in storm.

Charybdis - If the distance is detected to be less than 100 cm, charybdis is detected. The distance sensor has a range in which it gives accurate readings - around 2.8 cm to 300 cm. If the distance is either higher or lower, the sensor returns a value of around 330. To counteract such cases, an invalid distance is defined. If a reading is higher than invalidDist,i.e it is either out of the sensor's range or too far away to be able to trigger charybdis, previously measured values of distance are used to determine whether the distance is too low or too high and check whether the ship is in danger.

Danger start - the time at which the ship enters danger is recorded. If the ship remains in danger for more than 5 seconds, it is wrecked.

Note - if both storm and charybdis are entered at the EXACT same time, storm is arbitrarily chosen to take precedence. Also, the distance sensor makes tinkercad lag a lot and sometimes it takes more than 5 seconds.
