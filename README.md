# Parking Lot Simulator

### parking_lot_single_day

Analyses the data provided in a text file and identifies the busiest time interval on the parking lot, 
i.e. when the number of cars on the parking lot was at maximum.

#### Input

Data representing only one day of work is passed in a text file as a list in format:

        12:01 13:17
        03:04 11:56
        ...
    
Every row represents a single car’s parking start and end times.

#### Output

Maximum number of cars and time interval.

#### Examples

	10:10 11:10
	10:30 12:40 
	10:25 11:10
	11:10 12:20

	       10:00    10:10    10:25    10:30    11:10    12:20    12:40    
	CAR 1            xxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 2                              xxxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 3                     xxxxxxxxxxxxxxxxxx
	CAR 4                                       xxxxxxxxxx

Maximum number of cars: 3 at 10:30 - 11:10.

---

	09:00 15:00
	10:10 11:10
	10:30 12:40
	10:35 11:20
	10:25 11:10
	11:10 12:20

       09:00    10:10    10:25    10:30    10:35    11:10    11:20    12:20    12:40    15:00
	CAR 1   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 2            xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 3                              xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 4                                       xxxxxxxxxxxxxxxxxxx
	CAR 5                     xxxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 6                                                xxxxxxxxxxxxxxxxxxx

Maximum number of cars: 5 at 10:35 - 11:10.

---

	10:10, 11:10
	11:30, 12:40

	       10:00    10:10    10:25    10:30    11:10    11:30    12:40    
	CAR 1            xxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 2                                               xxxxxxxxxxx

Maximum number of cars: 1. No overlaing intervals.

---

	10:10 11:10
	11:10 12:40 

	       10:00    10:10    10:25    10:30    11:10    12:20    12:40    
	CAR 1            xxxxxxxxxxxxxxxxxxxxxxxxxxx
	CAR 2                                      xxxxxxxxxxxxxxxxxxxx
	
Maximum number of cars: 1. No overlaing intervals.

Note: In case two events coincide, leaving events are places before arrival events.

---

	09:00 10:25
	10:10 10:30
	10:35 11:10
	10:35 11:20

       09:00    10:10    10:25    10:30    10:35    11:10    11:20
	CAR 1   xxxxxxxxxxxxxxxxx
	CAR 2           xxxxxxxxxxxxxxxxxx
	CAR 3                                     xxxxxxxxxxxx
	CAR 4                                     xxxxxxxxxxxxxxxxxxxx

Maximum number of cars: 2 at 10:35 - 11:10.

Note: In case of equal number of overlaping intervals in different time frames,
the latest one is printed as output.

---

#### How to run

Navigate to parking_lot_single_day and run the following command:

```shell
g++ -std=c++11 parking_lot.cpp -o parking_lot
./parking_lot
```

### parking_lot_JSON
    
Analyses JSON data and identifies the busiest time on the parking lot, i.e. when the number of cars on the parking lot was at maximum.

#### Input

Data representing arbitrary number of work days is passed in the following format:

			[{
				"Id": 0,
				"ArrivalTime": "2016-05-01T06:36:00",
				"LeaveTime": "2016-05-01T13:28:00"
			}, 
			{
				"Id": 1,
				"ArrivalTime": "2016-05-01T14:17:00",
				"LeaveTime": "2016-05-02T01:35:00"
			}, 
			{
				"Id": 2,
				"ArrivalTime": "2016-05-04T00:15:00",
				"LeaveTime": "2016-05-04T06:55:00"
			}]

#### Output

Time interval with maximum number of cars for each date.

	Date: 2016-05-01
	Maximum numbers of cars 2 at 14:17-23:59.

	Date: 2016-05-04
	Maximum numbers of cars 3 at 00:15-23:59.

	Date: 2018-05-03
	Maximum numbers of cars 3 at 00:00-06:55.

	Date: 2018-05-04
	Maximum numbers of cars 2 at 00:00-13:28.

	Date: 2018-06-02
	Maximum numbers of car: 1 
	No overlapping intevals.

#### How to run

Navigate to parking_lot_JSON and run the following command:

```shell
g++ -std=c++11 parking_lot_json.cpp -o parking_lot_json
./parking_lot_json
```
---

### Visualization

Its possible to plot the maximum number of cars for each date. To generate the plot run:

```shell
chmod +x plot.sh
./plot.sh
```
Sample output image:

<p align="center">
	<img src="https://raw.githubusercontent.com/Dzvezdana/parking-lot-simulator/master/parking_lot_JSON/chart_1.png">  
</p>

### Requirements

* C++11
* nlohmann-json (required header file is already included)
* gnuplot for visualization (install using: apt-get install gnuplot)

Developed on Ubuntu 14.04.


