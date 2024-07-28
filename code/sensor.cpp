/**
 * @author Ian Guenther Green, Shubh Shah
 * @brief Implementation file for the sensor class that include functions and variables
 * Description: Sensor Singleton class that sets up PIR sensor and returns its read value for if it senses movement
 * Date: Nov 29, 2022
 */

#include "sensor.h"

// initialize singleton pointer
Sensor* Sensor::_instance = NULL;

/**
 * Returns singleton sensor instance.
 * @param none
 * @return pointer to insance of sensor class
 */
Sensor *Sensor::instance() {
    if(_instance == NULL)
        _instance = new Sensor();

    return _instance;

}

/**
 * Create a new sensor and set up hardware.
 * @brief Default constructor.
 * @param none
 */
Sensor::Sensor() {
    
    sensorPin = 0;
    disturbance = false;

    if(wiringPiSetup() == -1){
        printf("setup wiringPi failed !");
        exit(1);
    }

    pinMode(sensorPin,INPUT);

}

/**
 * Function to read the current sensor value.
 * @brief Read sensor.
 * @param none.
 * @return true if pir sensor detects movement, false otherwise.
 */
bool Sensor::getDisturbance(){
    return (digitalRead(sensorPin));
}
