/**
    Purpose: Analyses JSON data and identifies the busiest time on the parking lot, 
    i.e. when the number of cars on the parking lot was at maximum.

    Data representing arbitrary number of work days is passed in the following format:
    [
    	{
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
			}
		]

    Tested on Ubuntu 18.04. 

    To run use: g++ -std=c++11 parking_lot.cpp -o parking_lot

    @author Dzvezdana Arsovska
    @version 1.0 17/01/2018
*/

#include "json.hpp"
#include <iostream>
#include <algorithm> 
#include <vector>
#include <iterator>
#include <fstream>

// Used for processing JSON data.
using json = nlohmann::json;
int previousDayVisits = 0;

enum class EventType {ARRIVE, LEAVE};

// Used for preprocessing data.
struct Date 
{ 
    std::string date, time;
    EventType type;

    Date (std::string dt, std::string tm, EventType t) : date(dt), time (tm), type (t) {}

    bool operator<(const Date& d) const 
    { 
    	return (date < d.date); 
    }
};

// Used for the calculating of the overlaping intervals.
struct Event
{
    int time;
    EventType type;

    Event (int tm, EventType t) : time (tm), type (t) {}
    bool operator< (const Event& e) const
    {
        return (time < e.time) or (time == e.time and type == EventType::LEAVE and e.type != EventType::LEAVE);
    }
};

/**
    Calculates the maximum number of cars in the parking lot for each day and
    the time interval when this number was highest.

    @param events vector of arrival and leaving times
*/
void calculateResultingValues(std::vector<Event>& events)
{
    int maxVisits, localMaxVisits, localStartTime, localEndTime;
    std::vector<Event>::const_iterator it;
    localMaxVisits = 0;
    maxVisits = 0;
    bool arrive = true;
    bool depart = true;
    bool isArrive = true;
    bool isDepart = true;

    /* Take into account the number of cars that previously were
			 on the parking lot.
     */
    if(previousDayVisits != 0) 
    {
       for(int i = 0; i < previousDayVisits; i++) 
       {
        Event eStart(std::stoi("0"), EventType::ARRIVE);
        events.push_back(eStart);
       }
    }

    // Sorting the vector so that events are ordered by their arrival time.
    // In case two events coincide on arrival time, we place leaving events before arrival events.
    std::sort(events.begin(), events.end());

    // Find maximum number of visits.
    for (it = events.begin(); it != events.end(); it++)
    {
        if ((*it).type == EventType::ARRIVE)
        {
            localMaxVisits++;
            arrive = false;
        }
        else
        {
            maxVisits = std::max(maxVisits, localMaxVisits);
            localMaxVisits--;
            depart = false;
        }

        if(localMaxVisits == 0) maxVisits = 1;

        if(depart) maxVisits = localMaxVisits;
      
        if(arrive and localMaxVisits != 0) maxVisits = localMaxVisits;
    }

    previousDayVisits = localMaxVisits;

    localStartTime, localMaxVisits = 0;
    localEndTime = 0;

    if(maxVisits == 1) 
    {
        std::cout << "Maximum numbers of car: " << maxVisits << " " << std::endl;
        std::cout << "No overlapping intevals." << std::endl;
    }

    else
    {
        // Calculate the longest overlaping interval.
        for (it = events.begin(); it != events.end(); it++)
        {
            if ((*it).type == EventType::ARRIVE)
            {
                localMaxVisits++;
                if (localMaxVisits == maxVisits and localEndTime < (*it).time)
                { 
                    localStartTime = (*it).time;
                    isArrive = false;
                }
            }
            else
            {
                if (localMaxVisits == maxVisits)
                { 
                    localEndTime = (*it).time;
                    isDepart = false;
                }

                localMaxVisits--;
            }
        }

    if(isArrive) localStartTime = 0;
    if(isDepart) localEndTime = 2359;

    std::cout << "Maximum numbers of cars " << maxVisits << " ";

    std::string localStartTimeStr = std::to_string(localStartTime);
    std::string localEndTimeStr = std::to_string(localEndTime);

    if(localStartTimeStr.length() == 1) localStartTimeStr.insert(0,"000");
    if(localStartTimeStr.length() == 2) localStartTimeStr.insert(0,"00");
    if(localStartTimeStr.length() == 3) localStartTimeStr.insert(0,"0");
    if(localEndTimeStr.length() == 1) localEndTimeStr.insert(0,"000");
    if(localEndTimeStr.length() == 2) localEndTimeStr.insert(0,"00");
    if(localEndTimeStr.length() == 3) localEndTimeStr.insert(0,"0");

    std::cout << "at " << localStartTimeStr.insert((localStartTimeStr.length() - 2), ":") << "-" 
      << localEndTimeStr.insert((localEndTimeStr.length() - 2), ":") << "." << std::endl;
   }
}

/**
		Groups the times based on the dates.

    @param data vector of arrival and leaving times
    @return groups of times based on the date
*/
std::vector<std::vector<Date> > groupByIDs(const std::vector<Date>& data)
{
    std::vector<std::vector<Date> > groups;

    decltype(data.end()) upper;

    for(auto lower = data.begin(); lower != data.end(); lower = upper)
    {
        // Get the upper position of all elements with the same ID
        upper = std::upper_bound(data.begin(), data.end(), *lower);

        // Add those elements as a group to the output vector
        groups.emplace_back(lower, upper);
    }

    return groups;
}

/**
    Read the content from the file.

    @param fileName Input file
*/
void processJSONData(std::string fileName)
{
	std::string tmpData;
	std::vector<std::string> processData;
	std::vector<Date> timeEvents;
  std::ifstream ifs(fileName);
  json j = json::parse(ifs);

  // Extract the data from the JSON file.
  for (const auto& x : j.items())
   {
      for(auto& y : x.value())
      {
      	tmpData = y.dump();
      	processData.push_back(tmpData);
      }
   }

	// Separate the data based on the type of the event (leave or arrive).
	int cnt = 1;
	for (auto& it: processData) 
	{
		if(cnt == 1) 
			{
				Date eStart(it.substr(1,10), it.substr(12,5), EventType::ARRIVE);
				timeEvents.push_back(eStart);
			}

		if(cnt == 3) 
		{
			Date eEnd(it.substr(1,10), it.substr(12,5), EventType::LEAVE);
			timeEvents.push_back(eEnd);
		}

		cnt++;
		if(cnt > 3) {cnt = 1;}
	}

	// Sort the dates, starting from the oldest one.
	std::sort(timeEvents.begin(), timeEvents.end(), [](const Date& a, const Date &b)->bool {return a.date < b.date;}); 

	// Group the times based on the date.
	std::vector<std::vector<Date> > arrivalGroups = groupByIDs(timeEvents);

	int timeEvent;
	std::vector<Event> events;
	std::string processTime;

	// Call the calculating function for each group.
	for(auto const& group: arrivalGroups)
  {
      if(!group.empty())
          std::cout << "Date: " << group.front().date << '\n';

      for(auto const& d: group)
      {
          processTime = d.time;
        	processTime.erase(std::remove(processTime.begin(), processTime.end(), ':'), processTime.end());
        	timeEvent = std::stoi(processTime);
        	Event eTime(timeEvent, d.type);
        	events.push_back(eTime);
      }

     	calculateResultingValues(events);
     	events.clear();
     	std::cout << '\n';
  }
}

int main()
{
	processJSONData("test_data1.json");
}
