/**
 * @author Khanh Nguyen, Ian Guenther Green, Vicky Jiang
 * @brief header file for the alarm class that include functions and variables
 * Description: Alarm Singleton class that ring, stop, check status of the alarm
 * Date: Nov 29, 2022
 */

#ifndef ALARM_H
#define ALARM_H

#include <ctime>
#include <string>
#include <wiringPi.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "analytics.h"

class alarm
{
public:
    //public method to ring the alarm
    static void setAlarm(std::string startID);
    //public method to stop the alarm
    static void stopAlarm();
    //public method to get the current state of the alarm
    static bool checkAlarmState();
    //clean up
    static void Delete();

private:
    //constructor
    alarm(std::string descript, int pin);
    //desctuctor
    ~alarm() {};

    //static pointer to point to the alarm
    static alarm* ptrInstance;
    static alarm& instance(){
        //if there's no current alarm, create a new one
        if(!ptrInstance){
            ptrInstance = new alarm("LED alarm", 1);
        }
        return *ptrInstance;
    }

    //pin of the alarm
    int alarmPin;
    //start time of the alarm
    std::string startID;
    // end time of the alarm
    std::string end;
    //description of the alarm
    std::string description;
    //state of the alarm
    bool alarmState;

    //private method to ring the alarm
    void privateSetAlarm(std::string descript);
    //private method to stop the alarm
    void privateStopAlarm();
    //private method to get the alarm state
    bool privateCheckAlarmState();

};

#endif // ALARM_H
