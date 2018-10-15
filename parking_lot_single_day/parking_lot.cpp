/**
    Purpose: Analyses the data provided in text file and identifies the busiest time 
    on the parking lot, i.e. when the number of cars on the parking lot was at maximum.

    Data representing only one day of work is passed in a text file as a list in format:
        12:01 13:17
        03:04 11:56
        ...
    
    Every row represents a single car’s parking start and end times.

    Tested on Ubuntu 18.04. 

    To run use: g++ -std=c++11 parking_lot.cpp -o parking_lot

    @author Dzvezdana Arsovska
    @version 1.0 17/01/2018
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>

enum class EventType {ARRIVE, LEAVE};

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
    Calculates the maximum number of cars in the parking lot in one day and
    the time interval when this number was highest.

    @param events vector of arrival and leaving times
*/
void calculateResultingValues(std::vector<Event>& events)
{

    int maxVisits, localMaxVisits, localStartTime, localEndTime;
    std::vector<Event>::const_iterator it;

    // Sorting the vector so that events are ordered by their arrival time.
    // In case two events coincide on arrival time, we place leaving events before arrival events.
    std::sort(events.begin(), events.end());

    maxVisits = 0;
    localMaxVisits = 0;

    // Find maximum number of visits.
    for (it = events.begin(); it != events.end(); it++)
    {
        if ((*it).type == EventType::ARRIVE)
        {
            localMaxVisits++;
        }
        else
        {
            maxVisits = std::max(maxVisits, localMaxVisits);
            localMaxVisits--;
        }
    }

    localStartTime = 0;
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
                }
            }
            else
            {
                if (localMaxVisits == maxVisits)
                { 
                    localEndTime = (*it).time;
                }

                localMaxVisits--;
            }
        }

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
    Check if the input file is empty.

    @param pFile Input numbers
    @return true if the file is empty
*/
bool isEmpty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

/**
    Read the content from the file.

    @param pFile Input file
*/
void readFile(std::string fileName)
{
    std::ifstream infile(fileName);
    int timeStart, timeEnd;
    std::string col1, col2;
    std::vector<Event> events;

    if(!infile) 
    {
        std::cout << "Failed to open the file." << std::endl;
        exit(EXIT_FAILURE);
    }

    if(isEmpty(infile))
    {
        std::cout << "The file is empty." << std::endl;
        exit(EXIT_FAILURE);
    }

    // reads file to EOF
    while(!infile.eof())
    {
        
        /** Read all the visits, for each one save two entries to the events-vector,
            one for the arrival time another for the leaving time.
         */
        infile >> col1 >> col2;

        col1.erase(std::remove(col1.begin(), col1.end(), ':'), col1.end());
        col2.erase(std::remove(col2.begin(), col2.end(), ':'), col2.end());

        timeStart = std::stoi(col1);
        timeEnd = std::stoi(col2);

        Event eStart(timeStart, EventType::ARRIVE);
        Event eEnd(timeEnd, EventType::LEAVE);

        events.push_back(eStart);
        events.push_back(eEnd);
    }

    calculateResultingValues(events);

    infile.close();
}

int main()
{
    readFile("numbers.txt");
}