/**
 * @author Khanh Nguyen, Ian Guenther Green
 * @brief Implementation file for the homesystem that include functions and variables
 * Description: Homesystem Singleton class that set,get state of alarm and system, ring or stop alarm
 * Date: Nov 29, 2022
 */
using namespace std;

#include "homesystem.h"

//set pointer to the instance to null
homesystem* homesystem::ptrInstance = nullptr;

/**
 * Create a new homesystem.
 * @brief Default constructor.
 */
homesystem::homesystem()
{

}

/**
 * Private function to set the system state to activate or deactivated, also stop the alarm is system is deactiavted.
 * @author Khanh Nguyen, Ian Guenther Green
 * @brief Activate or deactivate the system.
 * @param Bool value to set for system state, true for activated and false for deactivated.
 * @return Set the system state.
 */
void homesystem::privateSetSystemState(bool privSysState){
    systemState = privSysState;
  	if(!privSysState) {
		alarm::stopAlarm();
    }
}

/**
 * Private function to get the system state.
 * @author Khanh Nguyen
 * @brief Get the state of the system.
 * @param none.
 * @return System state.
 */
bool homesystem::privateGetSystemState(){
    return systemState;
}

/**
 * Private function to set the alarm state to activate or deactivated, also stop the alarm is system is deactiavted.
 * @author Khanh Nguyen
 * @brief Activate or deactivate the alarm.
 * @param Bool value to set for alarm state, true for activated and false for deactivated.
 * @return Set the alarm state.
 */
void homesystem::privateSetAlarmState(bool privAlState){
    alarmState = privAlState;
}

/**
 * Private function to get the alarm state.
 * @author Khanh Nguyen, Ian Guenther Green
 * @brief Get the state of the alarm.
 * @param none.
 * @return Alarm state.
 */
bool homesystem::privateGetAlarmState(){
    return alarmState;
}

/**
 * Private function to ring the alarm.
 * @author Khanh Nguyen
 * @brief Ring the alarm.
 * @param none.
 * @return Ring alarm.
 */
void homesystem::privateRingAlarm(){
    alarm::setAlarm("triggered by homesystem");
}

/**
 * Private function to stop the alarm.
 * @author Khanh Nguyen
 * @brief Stop the alarm.
 * @param none.
 * @return Stop alarm.
 */
void homesystem::privateStopAlarm(){
    alarm::stopAlarm();
}

/**
 * Private function to initiate the control panel, analytics and start the thread for the sensor.
 * @author Khanh Nguyen,
 * @brief initiate the control panel, analytics and start the thread for the sensor.
 * @param argc, *argv[].
 * @return initiate the control panel, analytics and start the thread for the sensor.
 */
void homesystem::privateStartUp(int argc, char *argv[]){
    
    Analytics::Initialize();
    QApplication a(argc, argv);
    ControlPanel w;
    w.show();
    sThread = new sensorThread;
    sThread->start();
    a.exec();
}

/**
 * Private function to stop the thread of the sensor.
 * @author Khanh Nguyen
 * @brief stop the thread of the sensor.
 * @param none.
 * @return stop the thread of the sensor.
 */
void homesystem::privateStopSensor(){
    sThread = new sensorThread;
    sThread->exit = true;
}

/**
 * Private function to send analytics report over email
 * @author Ian Guenther Green
 * @brief sends alarm report.
 * @param none.
 * @return none.
 */
void homesystem::privateSendReport() {

    cout << "Sending Detection Report" << endl;
    
    NotifyUser notif;
    
    // retrieves detection report in a string and formats it for the correct input of emailUser()
    string detectionReportString = Analytics::RetrieveUnformattedTable("select * from AlarmRecords");
    char* detectionReport = const_cast<char*>(detectionReportString.c_str());

    notif.emailUser(detectionReport);
    
}

/**
 * Public function to set the system state by calling privateSetSystemState function.
 * @author Khanh Nguyen
 * @brief set the system state
 * @param true or false which stand for activated or deactivated.
 * @return set the system state.
 */
void homesystem::setSystemState(bool sysState){
    instance().privateSetSystemState(sysState);
}

/**
 * Public function to get the system state by calling privateGetSystemState function.
 * @author Khanh Nguyen
 * @brief get the system state
 * @param none.
 * @return system state.
 */
bool homesystem::getSystemState(){
    return instance().privateGetSystemState();
}

/**
 * Public function to set the alarm state by calling privateSetAlarmState function.
 * @author Khanh Nguyen
 * @brief set the alarm state
 * @param true or false which stand for activated or deactivated.
 * @return set the alarm state.
 */
void homesystem::setAlarmState(bool alState){
    instance().privateSetAlarmState(alState);
}

/**
 * Public function to get the alarm state by calling privateGetAlarmState function.
 * @author Khanh Nguyen
 * @brief get the alarm state
 * @param none.
 * @return alarm state.
 */
bool homesystem::getAlarmState(){
    return instance().privateGetAlarmState();
}

/**
 * Public function to ring the alarm state by calling privateRingAlarm function.
 * @author Khanh Nguyen
 * @brief ring the alarm
 * @param none.
 * @return ring the alarm.
 */
void homesystem::ringAlarm(){
    instance().privateRingAlarm();
}

/**
 * Public function to stop the alarm state by calling privateStopAlarm function.
 * @author Khanh Nguyen
 * @brief stop the alarm
 * @param none.
 * @return stop the alarm.
 */
void homesystem::stopAlarm(){
    instance().privateStopAlarm();
}

/**
 * Public function to clean up the application when exiting.
 * @author Khanh Nguyen
 * @brief clean up app
 * @param none.
 * @return clean up.
 */
void homesystem::Delete(){
	alarm::stopAlarm();
  	instance().privateSetSystemState(false);
	instance().privateStopSensor();
    delete ptrInstance;
    ptrInstance = nullptr;
    alarm::Delete();
}

/**
 * Public function to initiate the control panel, analytics and start the thread for the sensor by calling privateStartUp function.
 * @author Khanh Nguyen
 * @brief initiate the app
 * @param none.
 * @return initiate the app
 */
void homesystem::startUp(int argc, char *argv[]){
    instance().privateStartUp(argc, argv);
}

void homesystem::sendReport() {
    instance().privateSendReport();
}
