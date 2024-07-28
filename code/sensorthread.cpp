/**
 * @author Khanh Nguyen, Ian Guenther Green
 * @brief Implementation file for the thread of the sensor.
 * Description: Start the thread of the sensor so it runs while loop function waiting for signal from the sensor
 * Date: Nov 29, 2022
 */

#include <chrono>
#include <QtCore>
#include <iostream>
#include "notifyuser.h"

#include "homesystem.h"
#include "sensorthread.h"

using namespace std;

/**
 * Constructor to assign needed QT objects.
 * @brief Default constructor.
 * @param QObject parent
 */
sensorThread::sensorThread(QObject *parent) : QThread(parent) {

}

/**
 * function to run the thread for the loop to wait for sensor signal and send detection report by email.
 * @author Khanh Nguyen
 * @brief Get signal from the sensor.
 * @param none.
 * @return none.
 */
void sensorThread::run(){

    Sensor* sensor = Sensor::instance();
    NotifyUser notify;

    auto detection_start_time = std::chrono::steady_clock::now();
    auto current_time = std::chrono::steady_clock::now();

    bool sameDetection = false;
	//loop and wait for signal from the sensor, if received, ring the alarm and send detection report by email
    while (1){
        if (homesystem::getSystemState()){
            if (exit){
                homesystem::stopAlarm();
                break;
            }
            
            // if the sensor senses a movement
            if (sensor->getDisturbance()){
                homesystem::ringAlarm();

                // Send an email notification to the user if we are not in the sameDetection
                if(!sameDetection) {
                    detection_start_time = std::chrono::steady_clock::now();
                    notify.emailUser("EyeSpy Home Security System has detected a disturbance. \nThe home alarm has been activated");
                    sameDetection = true;
                }

                current_time = std::chrono::steady_clock::now();
                
                // only sends another email if it is a seperate disturbance
                if(std::chrono::duration_cast<std::chrono::seconds>(current_time - detection_start_time).count() > 15)
                    sameDetection = false;
                
            }
            
            this->msleep(10);
            
        }else if (!homesystem::getSystemState()){
            this->msleep(10);
        }

    }
}
