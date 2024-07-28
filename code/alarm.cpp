/**
 * @author Khanh Nguyen, Ian Guenther Green, Tabish Jabir, Vicky Jiang
 * @brief Implementation file for the alarm that include functions and variables
 * Description: Alarm class that ring the alarm, stop alarm, get the state of the alarm
 * Date: Nov 29, 2022
 */

using namespace std;
#include "alarm.h"

//allocate pointer
alarm* alarm::ptrInstance = nullptr;

/**
 * Create a new alarm and set up hardware.
 * @brief Default constructor.
 * @param pin of the alarm and the description
 */
alarm::alarm(string descript, int pin)
{
    description = descript;
    alarmPin = pin;
    startID = "";
    description = "";
    
    // initialize physical alarm pins
    wiringPiSetup();
    pinMode(alarmPin, OUTPUT);

}

/**
 * Private function to ring the alarm, record the information of the alarm start time.
 * @author Khanh Nguyen, Ian Guenther Green, Tabish Jabir
 * @brief Ring alarm and store data.
 * @param description of the alarm.
 * @return Ring alarm and store data.
 */
void alarm::privateSetAlarm(string descript){
    
    // only store a new start record if alarm wasn't already ringing
    if(!alarmState) {

        string TIMESTAMP; // initialize the variable for the incoming timestamp

        const auto now{chrono::system_clock::now()}; // get the present time and date
        const auto now_{chrono::system_clock::to_time_t(now)}; // convert the time and date to time_t format

        stringstream ss; // intialize a string stream to convert the timestamp into a string
        ss << put_time(localtime(&now_),"%Y-%m-%d %H:%M:%S"); // convert the timestamp into a string
        TIMESTAMP = ss.str();

        startID = TIMESTAMP;
        description = descript;

        Analytics::storeRecord(startID, "", descript, "new");
        
        cout << "Alarm Started at: " << TIMESTAMP << endl;

    }
    
    alarmState = true;
    digitalWrite(alarmPin, HIGH);
    
}

/**
 * Private function to stop the alarm, record the information of the alarm stop time.
 * @author Khanh Nguyen, Ian Guenther Green, Tabish Jabir
 * @brief Stop alarm and store data.
 * @param none.
 * @return Stop alarm and store data.
 */
void alarm::privateStopAlarm(){

    alarmState = false;
    digitalWrite(alarmPin, LOW);
    cout << "Alarm stopped" << endl;
    
    string TIMESTAMP; // initialize the variable for the incoming timestamp

    const auto now{chrono::system_clock::now()}; // get the present time and date
    const auto now_{chrono::system_clock::to_time_t(now)}; // convert the time and date to time_t format

    stringstream ss; // intialize a string stream to convert the timestamp into a string
    ss << put_time(localtime(&now_),"%Y-%m-%d %H:%M:%S"); // convert the timestamp into a string
    TIMESTAMP = ss.str();

    if (startID != ""){
        Analytics::storeRecord(startID, TIMESTAMP, description, "stop");

        startID = "";
        description = "";
    }
    
}

/**
 * Private function to get the state of the alarm.
 * @author Khanh Nguyen
 * @brief get the state of the alarm.
 * @param none.
 * @return get the state of the alarm.
 */
bool alarm::privateCheckAlarmState(){
    return alarmState;
}

/**
 * Private function to ring the alarm by calling privateRingAlarm.
 * @author Khanh Nguyen, Ian Guenther Green
 * @brief Ring alarm.
 * @param description of the alarm as start ID.
 * @return Ring alarm.
 */
void alarm::setAlarm(string startID){
    instance().privateSetAlarm(startID);
}

/**
 * Public function to stop the alarm by calling privateStopAlarm.
 * @author Khanh Nguyen, Ian Guenther Green
 * @brief Stop alarm
 * @param none.
 * @return Stop alarm .
 */
void alarm::stopAlarm(){
    instance().privateStopAlarm();
}

/**
 * Public function to get the state of the alarm by calling privateCheckAlarmState.
 * @author Khanh Nguyen
 * @brief get the state of the alarm.
 * @param none.
 * @return get the state of the alarm.
 */
bool alarm::checkAlarmState(){
    return instance().privateCheckAlarmState();
}

//delete object
void alarm::Delete(){
    delete ptrInstance;
    ptrInstance = nullptr;
}
